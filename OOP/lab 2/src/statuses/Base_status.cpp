#include "includes/Base_status.h"

int Base_status::get_amount() const {
	return 0;
}

bool Base_status::merge(const Robot_status& other) {
	return false;
}

bool Base_status::blocks_movement() const {
	return false;
}

bool Base_status::blocks_ability() const {
	return false;
}

void Base_status::on_ability_attempt() {
}

int Base_status::absorb_damage(int damage) {
	return damage;
}

int Base_status::on_turn_end() {
	return 0;
}

void Base_status::on_apply(Game_session& session, int robot_index) {
}

int Base_status::get_attack_bonus() const {
	return 0;
}
