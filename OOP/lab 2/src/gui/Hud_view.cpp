#include "includes/Hud_view.h"

#include "includes/Palette.h"

#include <string>
#include <vector>

using namespace std;

const SDL_Color kPanelColor = { 20, 20, 20, 255 };
const SDL_Color kBarColor = { 25, 25, 25, 255 };
const SDL_Color kSlotColor = { 45, 45, 55, 255 };
const SDL_Color kSelectedSlotColor = { 70, 60, 20, 255 };
const SDL_Color kLockedSlotColor = { 35, 35, 35, 255 };
const SDL_Color kSlotBorder = { 90, 90, 90, 255 };
const float kLineHeight = 20.0f;
// вертикальная позиция заголовка журнала
const float kLogTop = 330.0f;

Hud_view::Hud_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites)
	: canvas_(canvas), layout_(layout), sprites_(sprites) {
}

void Hud_view::draw(const Game_session& session, const Player_controller& controller,
					const Display_mode_selector& display_mode) {
	canvas_.fill_rect(layout_.get_side_panel(), kPanelColor);
	draw_player_stats(session, display_mode);
	draw_inventory(session);
	draw_log(session);

	canvas_.fill_rect(layout_.get_ability_bar(), kBarColor);
	for (int slot = 0; slot < controller.get_catalog().get_count(); slot++) {
		draw_ability_slot(session, controller, slot);
	}
	draw_hints(controller);
}

void Hud_view::draw_player_stats(const Game_session& session, const Display_mode_selector& display_mode) {
	const Robot& player = session.get_robot(Game_session::kPlayerIndex);
	float x = layout_.get_text_x();

	string attack = "Attack: " + to_string(player.get_attack_power());
	if (player.get_attack_bonus() > 0) {
		attack += " (+" + to_string(player.get_attack_bonus()) + ")";
	}
	canvas_.draw_title("Turn " + to_string(session.get_turn()), x, 10, Palette::kYellow);
	canvas_.draw_text("View: " + display_mode.get_name() + " [Tab]", x + 150, 17,
					  display_mode.get_mode() == Display_mode::Standard ? Palette::kGray : Palette::kYellow);
	canvas_.draw_title("HP: " + to_string(player.get_current_health()) + "/" + to_string(player.get_max_health()), x, 45, Palette::kWhite);
	canvas_.draw_title("Energy: " + to_string(player.get_current_energy()) + "/" + to_string(player.get_max_energy()), x, 75, Palette::kWhite);
	canvas_.draw_title("Moves: " + to_string(player.get_movement_points()) + "/" + to_string(player.get_speed()), x + 220, 75, Palette::kWhite);
	canvas_.draw_title(attack, x, 105, Palette::kWhite);
	canvas_.draw_title("Rank: " + to_string(player.get_rank()), x, 135, Palette::kWhite);
	canvas_.draw_title("XP: " + to_string(player.get_current_experience()) + "/"
					   + to_string(player.get_experience_to_next_rank()), x, 165, Palette::kWhite);

	string statuses = "Status:";
	for (int i = 0; i < player.get_status_count(); i++) {
		statuses += " " + player.get_status(i).get_description();
	}
	if (player.get_status_count() == 0) {
		statuses += " none";
	}
	canvas_.draw_text(statuses, x, 198, Palette::kWhite);
}

void Hud_view::draw_inventory(const Game_session& session) {
	const Inventory& inventory = session.get_inventory();
	float x = layout_.get_text_x();
	canvas_.draw_text("Inventory " + to_string(inventory.get_size()) + "/" + to_string(inventory.get_capacity())
					  + "  (keys 5-0 or click)", x, 222, Palette::kYellow);

	SDL_FPoint mouse;
	// под курсором показываем описание предмета
	SDL_GetMouseState(&mouse.x, &mouse.y);
	string hovered;
	for (int i = 0; i < inventory.get_capacity(); i++) {
		SDL_FRect slot = layout_.get_inventory_slot(i);
		canvas_.fill_rect(slot, kSlotColor);
		canvas_.draw_rect(slot, kSlotBorder);
		bool has_item = i < inventory.get_size();
		if (has_item) {
			const Item& item = inventory.get_item(i);
			SDL_FRect icon = { slot.x + 6, slot.y + 6, slot.w - 12, slot.h - 12 };
			sprites_.draw_item_icon(canvas_, item.get_name(), icon);
			if (SDL_PointInRectFloat(&mouse, &slot)) {
				hovered = item.get_name() + ": " + item.get_description();
				canvas_.draw_rect(slot, Palette::kYellow);
			}
		}
		// клавиши 5,6,7,8,9,0
		canvas_.draw_text(to_string((i + 5) % 10), slot.x + 3, slot.y + 1, has_item ? Palette::kWhite : Palette::kGray);
	}
	if (!hovered.empty()) {
		SDL_FRect first_slot = layout_.get_inventory_slot(0);
		canvas_.draw_text(hovered, x, first_slot.y + first_slot.h + 8, Palette::kWhite);
	}
}

void Hud_view::draw_log(const Game_session& session) {
	float x = layout_.get_text_x();
	canvas_.draw_title("Log:", x, kLogTop, Palette::kYellow);
	float top = kLogTop + 35;

	const vector<string>& entries = session.get_log().get_entries();
	// показываем только последние записи, которые помещаются в панель
	int max_lines = (int) ((layout_.get_board_height() - top - 10) / kLineHeight);
	int first = max(0, (int) entries.size() - max_lines);
	for (int i = first; i < (int) entries.size(); i++) {
		SDL_Color color = Palette::kWhite;
		// заголовки ходов — жёлтым, вложенные строки (с отступом) — серым
		if (entries[i].substr(0, 3) == "---") {
			color = Palette::kYellow;
		} else if (entries[i].substr(0, 2) == "  ") {
			color = Palette::kGray;
		}
		canvas_.draw_text(entries[i], x, top + (i - first) * kLineHeight, color);
	}
}

void Hud_view::draw_ability_slot(const Game_session& session, const Player_controller& controller, int slot) {
	SDL_FRect rect = layout_.get_ability_slot(slot);
	SDL_FRect icon = { rect.x + rect.w - 44, rect.y + 8, 36, 36 };
	const string& name = controller.get_catalog().get_name(slot);
	int ability_index = session.find_ability(Game_session::kPlayerIndex, name);

	// способность ещё не изучена — рисуем заблокированный слот
	if (ability_index < 0) {
		canvas_.fill_rect(rect, kLockedSlotColor);
		canvas_.draw_rect(rect, { 60, 60, 60, 255 });
		canvas_.draw_image(sprites_.get_ability_icon(name), nullptr, icon, { 255, 255, 255, 70 });
		canvas_.draw_text("[" + to_string(slot + 1) + "]", rect.x + 8, rect.y + 6, Palette::kGray);
		canvas_.draw_text(name, rect.x + 8, rect.y + 28, Palette::kGray);
		canvas_.draw_text("locked", rect.x + 8, rect.y + 52, Palette::kGray);
		return;
	}

	const Robot_abilities& ability = session.get_ability(Game_session::kPlayerIndex, ability_index);
	bool affordable = session.get_robot(Game_session::kPlayerIndex).get_current_energy() >= ability.get_energy_cost();
	bool selected = slot == controller.get_selected_slot();
	SDL_Color color = affordable ? Palette::kWhite : Palette::kGray;

	canvas_.fill_rect(rect, selected ? kSelectedSlotColor : kSlotColor);
	canvas_.draw_rect(rect, selected ? Palette::kYellow : kSlotBorder, selected ? 3 : 1);
	Uint8 shade = affordable ? 255 : 110;
	canvas_.draw_image(sprites_.get_ability_icon(name), nullptr, icon, { shade, shade, shade, 255 });
	canvas_.draw_text("[" + to_string(slot + 1) + "]  Lv " + to_string(ability.get_level()),
					  rect.x + 8, rect.y + 6, selected ? Palette::kYellow : color);
	canvas_.draw_text(ability.get_name(), rect.x + 8, rect.y + 28, color);
	canvas_.draw_text("Energy: " + to_string(ability.get_energy_cost()), rect.x + 8, rect.y + 52, color);
	canvas_.draw_text("Radius: " + to_string(ability.get_radius()), rect.x + 8, rect.y + 74, color);
}

void Hud_view::draw_hints(const Player_controller& controller) {
	float x = layout_.get_hint_x();
	float y = layout_.get_ability_slot(0).y - 4;
	bool switched = controller.is_ability_switched();
	canvas_.draw_text(switched ? "Switch: used this turn" : "Switch: available", x, y, switched ? Palette::kGray : Palette::kYellow);
	if (controller.is_targeting()) {
		canvas_.draw_text("Click target cell, Esc - cancel", x, y + 22, Palette::kYellow);
	} else {
		canvas_.draw_text("1-4 ability, Q use, 5-0 item", x, y + 22, Palette::kWhite);
	}
	canvas_.draw_text("WASD move, Enter end turn", x, y + 44, Palette::kWhite);
	draw_legend(x, y + 72);
}

void Hud_view::draw_legend(float x, float y) {
	const string names[] = { "Slow", "Overload", "Burn", "Shield", "Item" };
	for (const string& name : names) {
		SDL_FRect icon = { x, y + 1, 16, 16 };
		sprites_.draw_status_icon(canvas_, name, 0, icon);
		canvas_.draw_text(name, x + 19, y, sprites_.get_status_color(name));
		// грубая оценка ширины подписи: ~9 px на символ
		x += 19 + 9 * (float) name.size() + 12;
	}
}
