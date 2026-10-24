#include "includes/Long_attack.h"
#include "../game/includes/Game_session.h"
#include "../statuses/includes/Burn.h"

using namespace std;

const int kEnergyCost = 3;
const int kAttackDistance = 3;
const int kDamagePerLevel = 5;

Long_attack::Long_attack() : bonus_damage_(0), level_(1) {
}

string Long_attack::get_name() const{
	return "Long attack";
}

size_t Long_attack::get_energy_cost() const {
	return kEnergyCost;
}

int Long_attack::get_radius() const {
	return kAttackDistance;
}

bool Long_attack::needs_target() const {
	return true;
}

bool Long_attack::can_use(const Game_session& session, int user_index, int target_x, int target_y) const {
	if (!session.get_robots().get_robot(user_index).is_alive() || !session.get_robots().is_in_radius(user_index, target_x, target_y, kAttackDistance)) {
		return false;
	}
	int target_index = session.get_robots().find_at(target_x, target_y);
	// в целевой клетке должен стоять другой робот
	return target_index >= 0 && target_index != user_index;
}

void Long_attack::use(Game_session& session, int user_index, int target_x, int target_y) const {
	int target_index = session.get_robots().find_at(target_x, target_y);
	if (target_index < 0) {
		return;
	}
	int damage = (int) session.get_robots().get_robot(user_index).get_attack_power() + bonus_damage_;
	session.deal_damage(user_index, target_index, damage);
	session.apply_status(target_index, make_shared<Burn>());
}

int Long_attack::get_level() const {
	return level_;
}

string Long_attack::get_upgrade_description() const {
	return "bonus damage +" + to_string(bonus_damage_) + " -> +" + to_string(bonus_damage_ + kDamagePerLevel);
}

void Long_attack::upgrade() {
	bonus_damage_ += kDamagePerLevel;
	level_++;
}
