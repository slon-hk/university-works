#include "includes/Difficulty.h"

#include <algorithm>
#include <cstdlib>

using namespace std;

const int kEnemyPowerPercent[kDifficultyLevelCount] = { 75, 100, 150 };
const int kModifiersPerLevel[kDifficultyLevelCount] = { 0, 1, 2 };

Difficulty::Difficulty() : level_(Difficulty_level::Normal) {
}

Difficulty::Difficulty(Difficulty_level level) : level_(level) {
	// перемешиваем все модификаторы и берём первые несколько
	vector<Modifier> all;
	for (int i = 0; i < kModifierCount; i++) {
		all.push_back((Modifier) i);
	}
	for (int i = kModifierCount - 1; i > 0; i--) {
		swap(all[i], all[rand() % (i + 1)]);
	}
	all.resize(kModifiersPerLevel[(int) level]);
	modifiers_ = all;
}

Difficulty::Difficulty(Difficulty_level level, const vector<Modifier>& modifiers) : level_(level) {
	for (size_t i = 0; i < modifiers.size(); i++) {
		if (!has_modifier(modifiers[i])) {
			modifiers_.push_back(modifiers[i]);
		}
	}
}

Difficulty_level Difficulty::get_level() const {
	return level_;
}

string Difficulty::get_name() const {
	if (level_ == Difficulty_level::Easy) {
		return "Easy";
	}
	if (level_ == Difficulty_level::Hard) {
		return "Hard";
	}
	return "Normal";
}

int Difficulty::get_enemy_power_percent() const {
	return kEnemyPowerPercent[(int) level_];
}

bool Difficulty::has_modifier(Modifier modifier) const {
	return find(modifiers_.begin(), modifiers_.end(), modifier) != modifiers_.end();
}

const vector<Modifier>& Difficulty::get_modifiers() const {
	return modifiers_;
}

string Difficulty::get_modifier_description(Modifier modifier) const {
	if (modifier == Modifier::Single_ability_per_turn) {
		return "Only one ability use per turn";
	}
	if (modifier == Modifier::Random_status_each_turn) {
		return "Random status on the player every turn";
	}
	if (modifier == Modifier::Statuses_persist) {
		return "Statuses are kept between levels";
	}
	return "No ability upgrades on rank up";
}
