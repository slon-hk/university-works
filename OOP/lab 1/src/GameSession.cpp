#include "GameSession.h"

#include <cstdlib>

using namespace std;

GameSession::GameSession(GameBoard board) : board_(board) {
}

void GameSession::add_robot(Robot robot, int x, int y) {
    Unit unit = { robot, x, y };
    units_.push_back(unit);
}

void GameSession::add_fabric(Robot_factory factory, int x, int y) {
    Fabric fabric = {factory, x, y};
    fabric_.push_back(fabric);
}

bool GameSession::is_inside_board(int x, int y) const {
    if (x < 0 || y < 0) {
        return false;
    }
    pair<size_t, size_t> size = board_.get_board_size();
    int width = (int) size.first;
    int height = (int) size.second;
    if (x >= width || y >= height) {
        return false;
    }
    return true;
}

bool GameSession::is_passable(int x, int y) const {
    return !board_.get_square_border(x, y);
}

GameSession::Unit* GameSession::unit_at(int x, int y) {
    for (size_t i = 0; i < units_.size(); i++) {
        if (!units_[i].robot.is_alive()) {
            continue;
        }
        if (units_[i].x == x && units_[i].y == y) {
            return &units_[i];
        }
    }
    return nullptr;
}

bool GameSession::try_action(Unit& mover, Direction direction) {
    int new_x = mover.x;
    int new_y = mover.y;
    if (direction == Direction::UP) {
        new_y = new_y - 1;
    } else if (direction == Direction::DOWN) {
        new_y = new_y + 1;
    } else if (direction == Direction::LEFT) {
        new_x = new_x - 1;
    } else if (direction == Direction::RIGHT) {
        new_x = new_x + 1;
    }

    if (!is_inside_board(new_x, new_y)) {
        return false;
    }
    if (!is_passable(new_x, new_y)) {
        return false;
    }

    Unit* target = unit_at(new_x, new_y);
    int cost;
    if (target != nullptr) {
        cost = 1;
    } else {
        cost = board_.get_square_speed_level(new_x, new_y);
    }

    if ((int) mover.robot.get_current_energy() < cost) {
        return false;
    }

    if (target != nullptr) {
        mover.robot.change_energy(-cost);
        mover.robot.interaction_with_robot(&target->robot);
        return true;
    }

    mover.robot.change_energy(-cost);
    mover.x = new_x;
    mover.y = new_y;
    return true;
}

void GameSession::run_enemies_turn() {
    for (size_t i = 1; i < units_.size(); i++) {
        if (!units_[i].robot.is_alive()) {
            continue;
        }
        int value = rand() % 4;
        Direction direction;
        if (value == 0) {
            direction = Direction::UP;
        } else if (value == 1) {
            direction = Direction::DOWN;
        } else if (value == 2) {
            direction = Direction::LEFT;
        } else {
            direction = Direction::RIGHT;
        }
        try_action(units_[i], direction);
    }
}

void GameSession::regenerate_energy() {
    int energy_per_turn = 10;
    for (size_t i = 0; i < units_.size(); i++) {
        units_[i].robot.change_energy(energy_per_turn);
    }
}

bool GameSession::move_player(Direction direction) {
    if (units_.empty()) {
        return false;
    }
    bool acted = try_action(units_[0], direction);
    update_visibility();
    return acted;
}

bool GameSession::find_free_cell_around(int center_x, int center_y, int& out_x, int& out_y) {
    int dx[8] = { 0, 0, -1, 1, -1, -1, 1, 1 };
    int dy[8] = { -1, 1, 0, 0, -1, 1, -1, 1 };

    for (int i = 0; i < 8; i++) {
        int x = center_x + dx[i];
        int y = center_y + dy[i];
        if (!is_inside_board(x, y)) {
            continue;
        }
        if (!is_passable(x, y)) {
            continue;
        }
        if (unit_at(x, y) != nullptr) {
            continue;
        }
        out_x = x;
        out_y = y;
        return true;
    }
    return false;
}

void GameSession::spawn_from_factories() {
    for (size_t i = 0; i < fabric_.size(); i++) {
        vector<Robot> new_units = fabric_[i].fabric.produce();
        for (size_t j = 0; j < new_units.size(); j++) {
            int spawn_x;
            int spawn_y;
            if (find_free_cell_around(fabric_[i].x, fabric_[i].y, spawn_x, spawn_y)) {
                add_robot(new_units[j], spawn_x, spawn_y);
            }
        }
    }
}

void GameSession::end_turn() {
    run_enemies_turn();
    spawn_from_factories();
    regenerate_energy();
    update_visibility();
}

void GameSession::update_visibility() {
    if (units_.empty()) {
        return;
    }
    int player_x = units_[0].x;
    int player_y = units_[0].y;
    int radius = (int) units_[0].robot.get_vision_radius();

    pair<size_t, size_t> size = board_.get_board_size();
    int width = (int) size.first;
    int height = (int) size.second;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool in_view = (abs(x - player_x) <= radius && abs(y - player_y) <= radius);
            board_.set_square_visible(x, y, in_view);
            if (in_view && board_.get_square_border(x, y)) {
                board_.set_square_discovered(x, y, true);
            }
        }
    }
}

bool GameSession::is_level_complete() const {
    for (size_t i = 1; i < units_.size(); i++) {
        if (units_[i].robot.is_alive()) {
            return false;
        }
    }
    return true;
}

int GameSession::get_robot_count() const {
    return (int) units_.size();
}

int GameSession::get_robot_x(int index) const {
    return units_[index].x;
}

int GameSession::get_robot_y(int index) const {
    return units_[index].y;
}

int GameSession::get_robot_enemy(int index) const {
    return units_[index].robot.get_enemy();
}

bool GameSession::get_robot_alive(int index) const {
    return units_[index].robot.is_alive();
}

int GameSession::get_robot_current_health(int index) const {
    return (int) units_[index].robot.get_current_health();
}

int GameSession::get_robot_max_health(int index) const {
    return (int) units_[index].robot.get_max_health();
}

int GameSession::get_robot_current_energy(int index) const {
    return (int) units_[index].robot.get_current_energy();
}

int GameSession::get_robot_max_energy(int index) const {
    return (int) units_[index].robot.get_max_energy();
}

int GameSession::get_robot_attack_power(int index) const {
    return (int) units_[index].robot.get_attack_power();
}

int GameSession::get_robot_rank(int index) const {
    return (int) units_[index].robot.get_rank();
}

int GameSession::get_robot_current_experience(int index) const {
    return (int) units_[index].robot.get_current_experience();
}

int GameSession::get_robot_experience_to_next_rank(int index) const {
    return (int) units_[index].robot.get_experience_to_next_rank();
}

int GameSession::get_fabric_count() const {
    return (int) fabric_.size();
}

int GameSession::get_fabric_x(int index) const {
    return fabric_[index].x;
}

int GameSession::get_fabric_y(int index) const {
    return fabric_[index].y;
}

const GameBoard& GameSession::get_board() const {
    return board_;
}