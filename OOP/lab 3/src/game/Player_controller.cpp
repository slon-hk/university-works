#include "includes/Player_controller.h"

#include "../board/includes/Direction_offset.h"

using namespace std;

Player_controller::Player_controller(Game_session& session)
	: session_(session), selected_slot_(0), targeting_(false), cursor_x_(0), cursor_y_(0), ability_switched_(false),
	  level_complete_reported_(false) {
}

const Ability_catalog& Player_controller::get_catalog() const {
	return catalog_;
}

int Player_controller::get_selected_slot() const {
	return selected_slot_;
}

int Player_controller::get_selected_ability_index() const {
	return session_.get_robots().find_ability(Game_session::kPlayerIndex, catalog_.get_name(selected_slot_));
}

bool Player_controller::is_targeting() const {
	return targeting_;
}

int Player_controller::get_cursor_x() const {
	return cursor_x_;
}

int Player_controller::get_cursor_y() const {
	return cursor_y_;
}

bool Player_controller::is_ability_switched() const {
	return ability_switched_;
}

void Player_controller::restore(int selected_slot, bool ability_switched) {
	if (selected_slot >= 0 && selected_slot < catalog_.get_count()) {
		selected_slot_ = selected_slot;
	}
	ability_switched_ = ability_switched;
	targeting_ = false;
}

void Player_controller::choose_start_ability(int slot) {
	if (slot < 0 || slot >= catalog_.get_count() || session_.get_robots().get_ability_count(Game_session::kPlayerIndex) > 0) {
		return;
	}
	session_.add_ability(Game_session::kPlayerIndex, catalog_.create(slot));
	selected_slot_ = slot;
	session_.log_event("Start ability: " + catalog_.get_name(slot));
}

void Player_controller::select_ability(int slot) {
	if (slot < 0 || slot >= catalog_.get_count() || slot == selected_slot_) {
		return;
	}
	if (session_.get_robots().find_ability(Game_session::kPlayerIndex, catalog_.get_name(slot)) < 0) {
		session_.log_error(catalog_.get_name(slot) + " is locked");
		return;
	}
	if (ability_switched_) {
		session_.log_error("Ability can be switched once per turn");
		return;
	}
	selected_slot_ = slot;
	ability_switched_ = true;
	targeting_ = false;
	session_.log_event("Player selects " + catalog_.get_name(slot));
}

void Player_controller::activate_selected_ability() {
	int ability_index = get_selected_ability_index();
	if (ability_index < 0) {
		return;
	}
	if (targeting_) {
		use_ability_at(cursor_x_, cursor_y_);
		return;
	}
	if (session_.get_robots().get_ability(Game_session::kPlayerIndex, ability_index).needs_target()) {
		// прицельная способность — курсор появляется на клетке игрока
		targeting_ = true;
		cursor_x_ = session_.get_robots().get_x(Game_session::kPlayerIndex);
		cursor_y_ = session_.get_robots().get_y(Game_session::kPlayerIndex);
		return;
	}
	// без цели — применяем сразу, передавая клетку самого игрока
	use_ability_at(session_.get_robots().get_x(Game_session::kPlayerIndex), session_.get_robots().get_y(Game_session::kPlayerIndex));
}

void Player_controller::use_ability_at(int x, int y) {
	targeting_ = false;
	int ability_index = get_selected_ability_index();
	if (ability_index >= 0 && session_.use_ability(Game_session::kPlayerIndex, ability_index, x, y)) {
		after_action();
	}
}

void Player_controller::cancel_targeting() {
	targeting_ = false;
}

void Player_controller::move(Direction direction) {
	if (targeting_) {
		move_cursor(direction);
		return;
	}
	if (session_.move_player(direction)) {
		after_action();
	}
}

void Player_controller::move_cursor(Direction direction) {
	Direction_offset offset(direction);
	int x = cursor_x_ + offset.get_dx();
	int y = cursor_y_ + offset.get_dy();
	int ability_index = get_selected_ability_index();
	int radius = ability_index >= 0 ? session_.get_robots().get_ability(Game_session::kPlayerIndex, ability_index).get_radius() : 0;
	if (session_.is_inside_board(x, y) && session_.get_robots().is_in_radius(Game_session::kPlayerIndex, x, y, radius)) {
		cursor_x_ = x;
		cursor_y_ = y;
	}
}

void Player_controller::use_item(int slot) {
	session_.use_item(slot);
}

void Player_controller::choose_rank_reward(int slot) {
	if (slot < 0 || slot >= catalog_.get_count()) {
		return;
	}
	targeting_ = false;
	int ability_index = session_.get_robots().find_ability(Game_session::kPlayerIndex, catalog_.get_name(slot));
	if (ability_index < 0) {
		session_.learn_ability(catalog_.create(slot));
	} else {
		session_.upgrade_ability(ability_index);
	}
}

void Player_controller::end_turn() {
	targeting_ = false;
	ability_switched_ = false;
	session_.end_turn();
	report_level_complete();
}

bool Player_controller::can_act() const {
	const Robot& player = session_.get_robots().get_robot(Game_session::kPlayerIndex);
	if (player.get_movement_points() > 0) {
		return true;
	}
	if (session_.is_ability_limit_reached(Game_session::kPlayerIndex)) {
		return false;
	}
	for (int i = 0; i < session_.get_robots().get_ability_count(Game_session::kPlayerIndex); i++) {
		if (player.get_current_energy() >= session_.get_robots().get_ability(Game_session::kPlayerIndex, i).get_energy_cost()) {
			return true;
		}
	}
	return false;
}

void Player_controller::report_level_complete() {
	if (session_.is_level_complete() && !level_complete_reported_) {
		level_complete_reported_ = true;
		session_.log_event("Level complete!");
	}
}

void Player_controller::after_action() {
	report_level_complete();
	// делать больше нечего — завершаем ход автоматически
	if (!session_.is_game_over() && !can_act()) {
		end_turn();
	}
}
