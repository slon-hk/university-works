#include "includes/Ability_catalog.h"

#include <cstdlib>
#include <stdexcept>

#include "includes/Around_attack.h"
#include "includes/Heal.h"
#include "includes/Long_attack.h"
#include "includes/Teleport.h"

using namespace std;

const int kAbilityCount = 4;

Ability_catalog::Ability_catalog() {
	for (int i = 0; i < kAbilityCount; i++) {
		names_.push_back(create(i)->get_name());
	}
}

int Ability_catalog::get_count() const {
	return kAbilityCount;
}

const string& Ability_catalog::get_name(int index) const {
	return names_.at(index);
}

shared_ptr<Robot_abilities> Ability_catalog::create(int index) const {
	if (index == 0) {
		return make_shared<Around_attack>();
	}
	if (index == 1) {
		return make_shared<Long_attack>();
	}
	if (index == 2) {
		return make_shared<Heal>();
	}
	if (index == 3) {
		return make_shared<Teleport>();
	}
	return nullptr;
}

shared_ptr<Robot_abilities> Ability_catalog::create(const string& name, int level) const {
	if (level < 1 || level > kMaxLevel) {
		throw runtime_error("ability level " + to_string(level) + " is out of range");
	}
	for (int i = 0; i < kAbilityCount; i++) {
		if (names_[i] == name) {
			shared_ptr<Robot_abilities> ability = create(i);
			while (ability->get_level() < level) {
				ability->upgrade();
			}
			return ability;
		}
	}
	throw runtime_error("unknown ability '" + name + "'");
}

shared_ptr<Robot_abilities> Ability_catalog::create_enemy_ability() const {
	if (rand() % 2 == 0) {
		return make_shared<Heal>();
	}
	return make_shared<Teleport>();
}
