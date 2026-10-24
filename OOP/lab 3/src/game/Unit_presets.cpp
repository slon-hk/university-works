#include "includes/Unit_presets.h"

#include <algorithm>

using namespace std;

// прирост характеристик врага за каждый ранг сверх первого
const int kEnemyHealthPerRank = 20;
const int kEnemyAttackPerRank = 5;
const int kEnemyEnergyPerRank = 10;
const int kMaxEnemyVision = 9;
const int kMaxEnemySpeed = 4;
const int kFactoryHealthPerLevel = 25;
const int kMaxFactoryOutput = 3;
const int kMinSpawnPeriod = 2;

Unit_presets::Unit_presets(int power_percent) : power_percent_(max(power_percent, 1)) {
}

size_t Unit_presets::scale(size_t value) const {
	return max<size_t>(value * (size_t) power_percent_ / 100, 1);
}

Robot Unit_presets::create_enemy(int rank) const {
	size_t bonus = (size_t) max(rank - 1, 0);
	size_t health = scale(100 + kEnemyHealthPerRank * bonus);
	size_t energy = 10 + kEnemyEnergyPerRank * bonus;
	size_t vision = min<size_t>(5 + bonus / 2, kMaxEnemyVision);
	size_t speed = min<size_t>(2 + bonus / 3, kMaxEnemySpeed);
	// параметры: HP, макс. HP, атака, энергия, макс. энергия, опыт, опыт до ранга,
	// ранг, обзор, сила лечения, скорость, отношение
	return Robot(health, health, scale(10 + kEnemyAttackPerRank * bonus), energy, energy, 0, 100,
				 bonus + 1, vision, 10, speed, Relation::Enemy);
}

Robot Unit_presets::create_ally() const {
	return Robot(100, 100, 10, 10, 10, 0, 100, 1, 5, 10, 2, Relation::Ally);
}

Robot_factory Unit_presets::create_factory(int level) const {
	size_t bonus = (size_t) max(level - 1, 0);
	size_t unit_health = scale(60);
	Robot unit(unit_health, unit_health, scale(8), 10, 10, 0, 100, bonus + 1, 5, 10, 2, Relation::Enemy);
	size_t output = min<size_t>(1 + bonus / 3, kMaxFactoryOutput);
	size_t period = (size_t) max(5 - (int) bonus / 2, kMinSpawnPeriod);
	return Robot_factory(unit, output, bonus + 1, period,
						 scale(Robot_factory::kDefaultHealth + kFactoryHealthPerLevel * bonus));
}
