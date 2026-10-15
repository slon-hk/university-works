#include "includes/Dialog_view.h"

#include "includes/Palette.h"

#include <memory>
#include <string>

using namespace std;


Dialog_view::Dialog_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites)
	: canvas_(canvas), layout_(layout), sprites_(sprites) {
}

void Dialog_view::draw_start_screen(const Ability_catalog& catalog) {
	canvas_.clear(Palette::kBackground);
	canvas_.draw_title("Choose start ability:", 40, 40, Palette::kWhite);
	for (int i = 0; i < catalog.get_count(); i++) {
		shared_ptr<Robot_abilities> ability = catalog.create(i);
		float y = 100 + 40 * (float) i;
		SDL_FRect icon = { 40, y - 2, 30, 30 };
		canvas_.draw_image(sprites_.get_ability_icon(ability->get_name()), nullptr, icon);
		canvas_.draw_title(to_string(i + 1) + " - " + ability->get_name()
						   + " (energy " + to_string(ability->get_energy_cost()) + ")", 80, y, Palette::kWhite);
	}
	canvas_.draw_text("Other abilities are unlocked on rank up", 40, 280, Palette::kGray);
	canvas_.present();
}

void Dialog_view::draw_rank_reward(const Game_session& session, const Ability_catalog& catalog) {
	SDL_FRect window = { 0, 0, 560, 260 };
	window.x = (layout_.get_board_width() - window.w) / 2;
	window.y = (layout_.get_board_height() - window.h) / 2;
	canvas_.fill_rect(window, { 15, 15, 25, 255 });
	canvas_.draw_rect(window, Palette::kYellow);

	canvas_.draw_title("Rank up! Rank " + to_string(session.get_robot(Game_session::kPlayerIndex).get_rank()),
					   window.x + 20, window.y + 15, Palette::kYellow);
	canvas_.draw_text("Learn a new ability or upgrade one:", window.x + 20, window.y + 55, Palette::kWhite);

	for (int i = 0; i < catalog.get_count(); i++) {
		const string& name = catalog.get_name(i);
		int ability_index = session.find_ability(Game_session::kPlayerIndex, name);
		string text = to_string(i + 1) + " - ";
		SDL_Color color = Palette::kWhite;
		if (ability_index < 0) {
			text += "Learn " + name;
			color = Palette::kYellow;
		} else {
			const Robot_abilities& ability = session.get_ability(Game_session::kPlayerIndex, ability_index);
			text += "Upgrade " + name + " (Lv " + to_string(ability.get_level()) + "): "
				+ ability.get_upgrade_description();
		}
		float y = window.y + 90 + 35 * (float) i;
		SDL_FRect icon = { window.x + 20, y - 4, 26, 26 };
		canvas_.draw_image(sprites_.get_ability_icon(name), nullptr, icon);
		canvas_.draw_text(text, window.x + 54, y, color);
	}
}

void Dialog_view::draw_game_over(const Game_session& session) {
	SDL_FRect board = { 0, 0, layout_.get_board_width(), layout_.get_board_height() };
	canvas_.fill_rect(board, { 0, 0, 0, 170 });
	canvas_.draw_title("GAME OVER", board.w / 2 - 70, board.h / 2 - 30, { 255, 70, 60, 255 });
	canvas_.draw_text("Your robot was destroyed on turn " + to_string(session.get_turn()),
					  board.w / 2 - 150, board.h / 2 + 10, Palette::kWhite);
}


void Dialog_view::draw_level_complete(const Game_session& session) {
	SDL_FRect board = { 0, 0, layout_.get_board_width(), layout_.get_board_height() };
	canvas_.fill_rect(board, { 0, 0, 0, 170 });
	canvas_.draw_title("LEVEL COMPLETE", board.w / 2 - 110, board.h / 2 - 30, { 90, 220, 90, 255 });
	canvas_.draw_text("Cleared on turn " + to_string(session.get_turn()) + ". Press any key",
									  board.w / 2 - 150, board.h / 2 + 10, Palette::kWhite);
}