#include "Robot_factory.h"

using namespace std;

const int kHealthPerLevel = 20;
const int kAttackPerLevel = 5;
const int kEnergyPerLevel = 10;

Robot_factory::Robot_factory(const Robot &unit, size_t factory_output, size_t level)
    : level(level), unit(unit), factory_output(factory_output) {
}

void Robot_factory::set_level(size_t level) {
    this->level = level;
}
void Robot_factory::set_unit(const Robot &unit) {
    this->unit = unit;
}

void Robot_factory::set_factory_output(size_t factory_output) {
    this->factory_output = factory_output;
}

size_t Robot_factory::get_level() const {
    return level;
}

Robot Robot_factory::get_unit() const {
    return unit;
}

size_t Robot_factory::get_factory_output() const {
    return factory_output;
}

Robot Robot_factory::create() const {
    Robot new_unit = unit;

    int bonus_levels = 0;
    if (level > 1) {
        bonus_levels = (int) (level - 1);
    }

    int health_bonus = bonus_levels * kHealthPerLevel;
    int energy_bonus = bonus_levels * kEnergyPerLevel;
    int attack_bonus = bonus_levels * kAttackPerLevel;

    new_unit.change_max_health(health_bonus);
    new_unit.change_health(health_bonus);
    new_unit.change_max_energy(energy_bonus);
    new_unit.change_energy(energy_bonus);
    new_unit.change_attack_power(attack_bonus);

    return new_unit;
}

vector<Robot> Robot_factory::produce() {
    vector<Robot> arr_units;
    for (int i = 0; i < factory_output; i++) {
        arr_units.push_back((Robot_factory::create()));
    }
    return arr_units;
}