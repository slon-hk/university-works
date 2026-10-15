#include "includes/Gui.h"
#include "includes/Palette.h"
#include <string>

using namespace std;

// клавиши 1–4 — способности, 5–0 — слоты инвентаря
const int kAbilityKeys = 4;
const int kInventoryKeys = 6;
const SDL_Scancode kMenuKey = SDL_SCANCODE_M;

Gui::Gui(Game_session& session)
	: session_(session),
	  layout_((int) session.get_board().get_board_size().first, (int) session.get_board().get_board_size().second),
	  controller_(session), window_(nullptr), renderer_(nullptr), canvas_(nullptr),
	  board_view_(nullptr), hud_view_(nullptr), dialog_view_(nullptr) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("Game", layout_.get_window_width(), layout_.get_window_height(), 0,
								&window_, &renderer_);
	// FONT_PATH и ASSETS_PATH задаются в CMakeLists.txt
	canvas_ = new Canvas(renderer_, FONT_PATH, string(ASSETS_PATH) + "/Tiny Swords (Free Pack)");
	board_view_ = new Board_view(*canvas_, layout_, sprites_);
	hud_view_ = new Hud_view(*canvas_, layout_, sprites_);
	dialog_view_ = new Dialog_view(*canvas_, layout_, sprites_);
}

Gui::~Gui() {
	delete dialog_view_;
	delete hud_view_;
	delete board_view_;
	delete canvas_;
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

Level_result Gui::run() {
	if (session_.get_ability_count(Game_session::kPlayerIndex) == 0 && !run_start_screen()) {
		return Level_result::Quit;
	}

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				return Level_result::Quit;
			} else if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == kMenuKey) {
				return Level_result::Exit_to_menu;
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				handle_key(event.key.scancode);
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				handle_click(event.button.x, event.button.y);
			}
		}
		render();

		if (session_.is_game_over()) {
			return wait_for_key() ? Level_result::Lost : Level_result::Quit;
		}

		if (session_.is_level_complete() && !session_.has_rank_reward()) {
			return wait_for_key() ? Level_result::Completed : Level_result::Quit;
		}
	}
}

bool Gui::wait_for_key() {
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				return false;
			}
			if (event.type == SDL_EVENT_KEY_DOWN) {
				return true;
			}
		}
		render();
	}
}

bool Gui::run_start_screen() {
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				return false;
			}
			if (event.type == SDL_EVENT_KEY_DOWN) {
				// клавиши 1..4 → слоты 0..3
				controller_.choose_start_ability((int) event.key.scancode - (int) SDL_SCANCODE_1);
				if (session_.get_ability_count(Game_session::kPlayerIndex) > 0) {
					return true;
				}
			}
		}
		dialog_view_->draw_start_screen(controller_.get_catalog());
	}
}

void Gui::handle_key(SDL_Scancode key) {
	if (key == SDL_SCANCODE_TAB) {
		display_mode_.next();
		return;
	}
	if (session_.is_game_over()) {
		return;
	}

	int number = (int) key - (int) SDL_SCANCODE_1;
	bool is_number = number >= 0 && number < kAbilityKeys + kInventoryKeys;
	// пока открыт диалог награды за ранг, принимаем только выбор награды
	if (session_.has_rank_reward()) {
		if (is_number) {
			controller_.choose_rank_reward(number);
		}
		return;
	}

	if (key == SDL_SCANCODE_W) {
		controller_.move(Direction::UP);
	} else if (key == SDL_SCANCODE_S) {
		controller_.move(Direction::DOWN);
	} else if (key == SDL_SCANCODE_A) {
		controller_.move(Direction::LEFT);
	} else if (key == SDL_SCANCODE_D) {
		controller_.move(Direction::RIGHT);
	} else if (key == SDL_SCANCODE_Q) {
		controller_.activate_selected_ability();
	} else if (key == SDL_SCANCODE_ESCAPE) {
		controller_.cancel_targeting();
	} else if (key == SDL_SCANCODE_RETURN) {
		controller_.end_turn();
	} else if (is_number && number < kAbilityKeys) {
		controller_.select_ability(number);
	} else if (is_number) {
		controller_.use_item(number - kAbilityKeys);
	}
}

void Gui::handle_click(float x, float y) {
	if (session_.is_game_over() || session_.has_rank_reward()) {
		return;
	}
	SDL_FPoint point = { x, y };
	for (int i = 0; i < session_.get_inventory().get_capacity(); i++) {
		SDL_FRect slot = layout_.get_inventory_slot(i);
		if (SDL_PointInRectFloat(&point, &slot)) {
			controller_.use_item(i);
			return;
		}
	}
	int cell_x;
	int cell_y;
	if (controller_.is_targeting() && layout_.find_cell(x, y, cell_x, cell_y)) {
		controller_.use_ability_at(cell_x, cell_y);
	}
}

void Gui::render() {
	canvas_->clear(Palette::kBackground);
	board_view_->draw(session_, controller_, display_mode_);
	hud_view_->draw(session_, controller_, display_mode_);
	if (session_.has_rank_reward()) {
		dialog_view_->draw_rank_reward(session_, controller_.get_catalog());
	}
	if (session_.is_level_complete() && session_.has_rank_reward()) {
		dialog_view_->draw_level_complete(session_);
	}
	if (session_.is_game_over()) {
		dialog_view_->draw_game_over(session_);
	}
	canvas_->present();
}
