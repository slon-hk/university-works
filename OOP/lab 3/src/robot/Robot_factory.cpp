#include "includes/Robot_factory.h"

#include <algorithm>

using namespace std;

// прибавка характеристик за каждый уровень завода сверх первого
const int kHealthPerLevel = 20;
const int kAttackPerLevel = 5;
const int kEnergyPerLevel = 10;
// завод занимает квадрат 2x2
const int kFactorySize = 2;

Robot_factory::Robot_factory(const Robot& unit, size_t factory_output, size_t level, size_t spawn_period,
							 size_t max_health)
	: unit_(unit), factory_output_(factory_output > 0 ? factory_output : 1), level_(level > 0 ? level : 1),
	  spawn_period_(spawn_period > 0 ? spawn_period : 1), turns_until_spawn_(spawn_period_),
	  current_health_(max_health > 0 ? max_health : 1), max_health_(current_health_) {
}

void Robot_factory::set_level(size_t level) {
	level_ = level > 0 ? level : 1;
}

void Robot_factory::set_unit(const Robot& unit) {
	unit_ = unit;
}

void Robot_factory::set_factory_output(size_t factory_output) {
	factory_output_ = factory_output > 0 ? factory_output : 1;
}

void Robot_factory::set_spawn_period(size_t spawn_period) {
	spawn_period_ = spawn_period > 0 ? spawn_period : 1;
	// чтобы при уменьшении периода не ждать дольше нового периода
	turns_until_spawn_ = min(turns_until_spawn_, spawn_period_);
}

size_t Robot_factory::get_level() const {
	return level_;
}

Robot Robot_factory::get_unit() const {
	return unit_;
}

size_t Robot_factory::get_factory_output() const {
	return factory_output_;
}

size_t Robot_factory::get_spawn_period() const {
	return spawn_period_;
}

void Robot_factory::set_turns_until_spawn(size_t turns) {
	turns_until_spawn_ = min(max<size_t>(turns, 1), spawn_period_);
}

size_t Robot_factory::get_turns_until_spawn() const {
	return turns_until_spawn_;
}

int Robot_factory::get_size() const {
	return kFactorySize;
}

size_t Robot_factory::get_current_health() const {
	return current_health_;
}

size_t Robot_factory::get_max_health() const {
	return max_health_;
}

void Robot_factory::take_damage(int damage) {
	if (damage <= 0) {
		return;
	}
	current_health_ = (size_t) damage >= current_health_ ? 0 : current_health_ - damage;
}

bool Robot_factory::is_destroyed() const {
	return current_health_ == 0;
}

Robot Robot_factory::create() const {
	Robot new_unit = unit_;
	// первый уровень бонусов не даёт
	int bonus_levels = (int) level_ - 1;

	new_unit.change_max_health(bonus_levels * kHealthPerLevel);
	new_unit.change_health(bonus_levels * kHealthPerLevel);
	new_unit.change_max_energy(bonus_levels * kEnergyPerLevel);
	new_unit.change_energy(bonus_levels * kEnergyPerLevel);
	new_unit.change_attack_power(bonus_levels * kAttackPerLevel);
	return new_unit;
}

vector<Robot> Robot_factory::produce() {
	vector<Robot> units;
	if (is_destroyed()) {
		return units;
	}
	turns_until_spawn_--;
	if (turns_until_spawn_ > 0) {
		return units;
	}
	// время пришло — перезапускаем таймер и выпускаем партию
	turns_until_spawn_ = spawn_period_;
	for (size_t i = 0; i < factory_output_; i++) {
		units.push_back(create());
	}
	return units;
}
