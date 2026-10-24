#include "includes/Dialog_view.h"

#include "includes/Palette.h"

#include <memory>
#include <string>

using namespace std;


Dialog_view::Dialog_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites,
						 const Key_labels& labels)
	: canvas_(canvas), layout_(layout), sprites_(sprites), labels_(labels) {
}

void Dialog_view::draw_menu(bool has_save) {
	canvas_.clear(Palette::kBackground);
	canvas_.draw_title("Main menu", 40, 40, Palette::kYellow);
	float y = 100;
	for (int i = 0; i < modes_.get_count(); i++) {
		canvas_.draw_title(labels_.get_ability(i) + " - New game: " + modes_.get_title(i), 40, y, Palette::kWhite);
		y += 40;
	}
	// пункт загрузки идёт сразу после режимов
	canvas_.draw_title(labels_.get_ability(modes_.get_count()) + " / " + labels_.get(Command::Load)
					   + " - Load saved game", 40, y, has_save ? Palette::kWhite : Palette::kGray);
	canvas_.draw_title(labels_.get(Command::Cancel) + " - Quit", 40, y + 40, Palette::kWhite);
	if (!has_save) {
		canvas_.draw_text("No saved game yet", 40, y + 90, Palette::kGray);
	}
	canvas_.draw_text("In game: " + labels_.get(Command::Save) + " save, " + labels_.get(Command::Load) + " load, "
					  + labels_.get(Command::Menu) + " return here", 40, y + 120, Palette::kGray);
	canvas_.present();
}

void Dialog_view::draw_difficulty_menu(int mode) {
	canvas_.clear(Palette::kBackground);
	canvas_.draw_title("New game: " + modes_.get_title(mode) + ". Choose difficulty", 40, 40, Palette::kYellow);
	const string descriptions[] = { "weaker enemies, no modifiers", "normal enemies, 1 random modifier",
									"stronger enemies, 2 random modifiers" };
	for (int i = 0; i < kDifficultyLevelCount; i++) {
		Difficulty_level level = (Difficulty_level) i;
		Difficulty difficulty(level, {});
		canvas_.draw_title(labels_.get_ability(i) + " - " + difficulty.get_name(), 40, 100 + 40 * (float) i,
						   Palette::kWhite);
		canvas_.draw_text("enemy power " + to_string(difficulty.get_enemy_power_percent()) + "%, "
						  + descriptions[i], 220, 106 + 40 * (float) i, Palette::kGray);
	}
	canvas_.draw_title(labels_.get(Command::Cancel) + " - Back", 40, 100 + 40 * (float) kDifficultyLevelCount,
					   Palette::kWhite);
	canvas_.draw_text("Possible modifiers:", 40, 280, Palette::kGray);
	for (int i = 0; i < kModifierCount; i++) {
		canvas_.draw_text("- " + Difficulty().get_modifier_description((Modifier) i), 60, 305 + 22 * (float) i,
						  Palette::kGray);
	}
	canvas_.present();
}

void Dialog_view::draw_message(const string& title, const string& text) {
	canvas_.clear(Palette::kBackground);
	canvas_.draw_title(title, 40, 40, Palette::kYellow);
	// текст выводится построчно по символам '\n'
	float y = 100;
	size_t start = 0;
	while (start <= text.size()) {
		size_t end = text.find('\n', start);
		if (end == string::npos) {
			end = text.size();
		}
		canvas_.draw_text(text.substr(start, end - start), 40, y, Palette::kWhite);
		y += 25;
		start = end + 1;
	}
	canvas_.draw_text("Press any control key", 40, y + 15, Palette::kGray);
	canvas_.present();
}

void Dialog_view::draw_start_screen(const Ability_catalog& catalog, const Difficulty& difficulty) {
	canvas_.clear(Palette::kBackground);
	canvas_.draw_title("Choose start ability:", 40, 40, Palette::kWhite);
	for (int i = 0; i < catalog.get_count(); i++) {
		shared_ptr<Robot_abilities> ability = catalog.create(i);
		float y = 100 + 40 * (float) i;
		SDL_FRect icon = { 40, y - 2, 30, 30 };
		canvas_.draw_image(sprites_.get_ability_icon(ability->get_name()), nullptr, icon);
		canvas_.draw_title(labels_.get_ability(i) + " - " + ability->get_name()
						   + " (energy " + to_string(ability->get_energy_cost()) + ")", 80, y, Palette::kWhite);
	}
	canvas_.draw_text("Other abilities are unlocked on rank up", 40, 280, Palette::kGray);

	canvas_.draw_title("Difficulty: " + difficulty.get_name(), 40, 330, Palette::kYellow);
	if (difficulty.get_modifiers().empty()) {
		canvas_.draw_text("No modifiers", 40, 370, Palette::kGray);
	}
	for (size_t i = 0; i < difficulty.get_modifiers().size(); i++) {
		canvas_.draw_text("- " + difficulty.get_modifier_description(difficulty.get_modifiers()[i]), 40,
						  370 + 24 * (float) i, Palette::kWhite);
	}
	canvas_.present();
}

void Dialog_view::draw_rank_reward(const Game_session& session, const Ability_catalog& catalog) {
	SDL_FRect window = { 0, 0, 560, 260 };
	window.x = (layout_.get_board_width() - window.w) / 2;
	window.y = (layout_.get_board_height() - window.h) / 2;
	canvas_.fill_rect(window, { 15, 15, 25, 255 });
	canvas_.draw_rect(window, Palette::kYellow);

	canvas_.draw_title("Rank up! Rank " + to_string(session.get_robots().get_robot(Game_session::kPlayerIndex).get_rank()),
					   window.x + 20, window.y + 15, Palette::kYellow);
	canvas_.draw_text("Learn a new ability or upgrade one:", window.x + 20, window.y + 55, Palette::kWhite);

	for (int i = 0; i < catalog.get_count(); i++) {
		const string& name = catalog.get_name(i);
		int ability_index = session.get_robots().find_ability(Game_session::kPlayerIndex, name);
		string text = labels_.get_ability(i) + " - ";
		SDL_Color color = Palette::kWhite;
		if (ability_index < 0) {
			text += "Learn " + name;
			color = Palette::kYellow;
		} else if (session.get_difficulty().has_modifier(Modifier::No_ability_upgrades)) {
			text += name + ": upgrades disabled";
			color = Palette::kGray;
		} else {
			const Robot_abilities& ability = session.get_robots().get_ability(Game_session::kPlayerIndex, ability_index);
			text += "Upgrade " + name + " (Lv " + to_string(ability.get_level()) + "): "
				+ ability.get_upgrade_description();
		}
		float y = window.y + 90 + 35 * (float) i;
		SDL_FRect icon = { window.x + 20, y - 4, 26, 26 };
		canvas_.draw_image(sprites_.get_ability_icon(name), nullptr, icon);
		canvas_.draw_text(text, window.x + 54, y, color);
	}
}

void Dialog_view::draw_game_over(const Game_session& session, int level) {
	SDL_FRect board = { 0, 0, layout_.get_board_width(), layout_.get_board_height() };
	canvas_.fill_rect(board, { 0, 0, 0, 170 });
	canvas_.draw_title("GAME OVER", board.w / 2 - 70, board.h / 2 - 30, { 255, 70, 60, 255 });
	canvas_.draw_text("Your robot was destroyed on level " + to_string(level) + ", turn "
					  + to_string(session.get_turn()) + ". Press any key",
					  board.w / 2 - 150, board.h / 2 + 10, Palette::kWhite);
}


void Dialog_view::draw_level_complete(const Game_session& session, int level) {
	SDL_FRect board = { 0, 0, layout_.get_board_width(), layout_.get_board_height() };
	canvas_.fill_rect(board, { 0, 0, 0, 170 });
	canvas_.draw_title("LEVEL " + to_string(level) + " COMPLETE", board.w / 2 - 125, board.h / 2 - 30, { 90, 220, 90, 255 });
	canvas_.draw_text("Cleared on turn " + to_string(session.get_turn()) + ". Press any key",
									  board.w / 2 - 150, board.h / 2 + 10, Palette::kWhite);
}