#include "GUI.h"

#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

const float kCellSize = 20.0f;

Gui::Gui(GameSession& session) : session_(session) {
    window_ = nullptr;
    renderer_ = nullptr;
    font_ = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("Game", 800, 640, 0, &window_, &renderer_);
    TTF_Init();
    font_ = TTF_OpenFont(FONT_PATH, 24.0f);
    if (font_ == nullptr) {
        cout << "Не удалось загрузить шрифт: " << SDL_GetError() << endl;
    }
}

Gui::~Gui() {
    if (font_ != nullptr) {
        TTF_CloseFont(font_);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Gui::draw_text(const string& text, float x, float y) {
    if (font_ == nullptr) {
        return;
    }

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Blended(font_, text.c_str(), text.size(), color);
    if (surface == nullptr) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);

    SDL_FRect destination;
    destination.x = x;
    destination.y = y;
    destination.w = surface->w;
    destination.h = surface->h;

    SDL_RenderTexture(renderer_, texture, nullptr, &destination);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void Gui::draw_board() {
    const GameBoard& board = session_.get_board();
    vector<pair<int, int>> cells = board.get_square_cord();

    for (size_t i = 0; i < cells.size(); i++) {
        int x = cells[i].first;
        int y = cells[i].second;

        SDL_FRect cell;
        cell.x = x * kCellSize;
        cell.y = y * kCellSize;
        cell.w = kCellSize;
        cell.h = kCellSize;

        if (board.get_square_visible(x, y)) {
            if (board.get_square_border(x, y)) {
                SDL_SetRenderDrawColor(renderer_, 50, 50, 50, 255);
            } else {
                int green = (int) (150 / board.get_square_speed_level(x, y));
                SDL_SetRenderDrawColor(renderer_, 80, green, 80, 255);
            }
        } else if (board.get_square_discovered(x, y)) {
            SDL_SetRenderDrawColor(renderer_, 50, 50, 50, 255);
        } else {
            SDL_SetRenderDrawColor(renderer_, 15, 15, 15, 255);
        }
        SDL_RenderFillRect(renderer_, &cell);

        SDL_SetRenderDrawColor(renderer_, 40, 40, 40, 255);
        SDL_RenderRect(renderer_, &cell);
    }
}

void Gui::draw_factories() {
    const GameBoard& board = session_.get_board();

    for (int i = 0; i < session_.get_fabric_count(); i++) {
        int x = session_.get_fabric_x(i);
        int y = session_.get_fabric_y(i);
        if (!board.get_square_visible(x, y)) {
            continue;
        }

        SDL_FRect cell;
        cell.x = x * kCellSize;
        cell.y = y * kCellSize;
        cell.w = kCellSize;
        cell.h = kCellSize;

        SDL_SetRenderDrawColor(renderer_, 230, 200, 40, 255);
        SDL_RenderFillRect(renderer_, &cell);
    }
}

void Gui::draw_robots() {
    const GameBoard& board = session_.get_board();

    for (int i = 0; i < session_.get_robot_count(); i++) {
        if (!session_.get_robot_alive(i)) {
            continue;
        }
        int x = session_.get_robot_x(i);
        int y = session_.get_robot_y(i);
        if (!board.get_square_visible(x, y)) {
            continue;
        }

        SDL_FRect cell;
        cell.x = x * kCellSize;
        cell.y = y * kCellSize;
        cell.w = kCellSize;
        cell.h = kCellSize;

        if (session_.get_robot_enemy(i) < 0) {
            SDL_SetRenderDrawColor(renderer_, 255, 40, 40, 255);
        } else {
            SDL_SetRenderDrawColor(renderer_, 40, 40, 255, 255);
        }
        SDL_RenderFillRect(renderer_, &cell);

        if (i != 0) {
            string label = to_string(session_.get_robot_current_health(i))
                + "/" + to_string(session_.get_robot_attack_power(i));
            draw_text(label, cell.x, cell.y - kCellSize);
        }
    }
}

void Gui::draw_stats() {
    if (session_.get_robot_count() == 0) {
        return;
    }
    int player = 0;

    SDL_FRect panel;
    panel.x = 0;
    panel.y = 0;
    panel.w = 240;
    panel.h = 160;
    SDL_SetRenderDrawColor(renderer_, 20, 20, 20, 255);
    SDL_RenderFillRect(renderer_, &panel);

    string health = "HP: "
        + to_string(session_.get_robot_current_health(player)) + "/"
        + to_string(session_.get_robot_max_health(player));
    string energy = "Energy: "
        + to_string(session_.get_robot_current_energy(player)) + "/"
        + to_string(session_.get_robot_max_energy(player));
    string attack = "Attack: "
        + to_string(session_.get_robot_attack_power(player));
    string rank = "Rank: "
        + to_string(session_.get_robot_rank(player));
    string experience = "XP: "
        + to_string(session_.get_robot_current_experience(player)) + "/"
        + to_string(session_.get_robot_experience_to_next_rank(player));

    draw_text(health, 5, 5);
    draw_text(energy, 5, 35);
    draw_text(attack, 5, 65);
    draw_text(rank, 5, 95);
    draw_text(experience, 5, 125);
}

void Gui::render() {
    SDL_SetRenderDrawColor(renderer_, 30, 30, 30, 255);
    SDL_RenderClear(renderer_);

    draw_board();
    draw_factories();
    draw_robots();
    draw_stats();

    SDL_RenderPresent(renderer_);
}

void Gui::run() {
    session_.update_visibility();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                SDL_Scancode key = event.key.scancode;

                Direction direction = Direction::UP;
                bool has_direction = true;
                if (key == SDL_SCANCODE_W) {
                    direction = Direction::UP;
                } else if (key == SDL_SCANCODE_S) {
                    direction = Direction::DOWN;
                } else if (key == SDL_SCANCODE_A) {
                    direction = Direction::LEFT;
                } else if (key == SDL_SCANCODE_D) {
                    direction = Direction::RIGHT;
                } else {
                    has_direction = false;
                }

                if (has_direction) {
                    session_.move_player(direction);
                    if (session_.is_level_complete()) {
                        cout << "Level complete!" << endl;
                    }

                    if (session_.get_robot_current_energy(0) == 0) {
                        session_.end_turn();
                    }
                } else if (key == SDL_SCANCODE_RETURN) {
                    session_.end_turn();
                    if (session_.is_level_complete()) {
                        cout << "Level complete!" << endl;
                    }
                }
            }
        }

        render();
    }
}