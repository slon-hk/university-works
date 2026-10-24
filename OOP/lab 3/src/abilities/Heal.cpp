#include "includes/Heal.h"
#include "../game/includes/Game_session.h"
#include "../statuses/includes/Shield.h"

using namespace std;

const int kEnergyCost = 5;
// действует только на самого робота
const int kHealRadius = 0;
const int kStartHealAmount = 30;
const int kHealPerLevel = 15;

Heal::Heal() : heal_amount_(kStartHealAmount), level_(1) {
}

string Heal::get_name() const {
	return "Heal";
}

size_t Heal::get_energy_cost() const {
	return kEnergyCost;
}

int Heal::get_radius() const {
	return kHealRadius;
}

bool Heal::needs_target() const {
	return false;
}

bool Heal::can_use(const Game_session &session, int user_index, int target_x, int target_y) const{
	return session.get_robots().get_robot(user_index).is_alive();
}

void Heal::use(Game_session &session, int user_index, int target_x, int target_y) const {
	const Robot& user = session.get_robots().get_robot(user_index);
	// запоминаем недостающее здоровье до лечения, чтобы посчитать излишек
	int missing = (int) user.get_max_health() - (int) user.get_current_health();
	session.heal_robot(user_index, heal_amount_);

	// лечение сверх максимума HP превращается в щит
	int overflow = heal_amount_ - missing;
	if (overflow > 0) {
		session.apply_status(user_index, make_shared<Shield>(overflow));
	}
}

int Heal::get_level() const {
	return level_;
}

string Heal::get_upgrade_description() const {
	return "heal " + to_string(heal_amount_) + " -> " + to_string(heal_amount_ + kHealPerLevel);
}

void Heal::upgrade() {
	heal_amount_ += kHealPerLevel;
	level_++;
}
