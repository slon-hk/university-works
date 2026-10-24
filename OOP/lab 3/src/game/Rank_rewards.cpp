#include "includes/Rank_rewards.h"

#include <string>

using namespace std;

Rank_rewards::Rank_rewards() : rewarded_rank_(0) {
}

int Rank_rewards::get_rewarded_rank() const {
	return rewarded_rank_;
}

void Rank_rewards::set_rewarded_rank(int rank) {
	rewarded_rank_ = rank;
}

bool Rank_rewards::is_available(const Robot_roster& robots, const Difficulty& difficulty) const {
	if (robots.is_empty()) {
		return false;
	}
	const Robot& player = robots.get_robot(Robot_roster::kPlayerIndex);
	return player.is_alive() && (int) player.get_rank() > rewarded_rank_ && has_choice(robots, difficulty);
}

bool Rank_rewards::has_choice(const Robot_roster& robots, const Difficulty& difficulty) const {
	if (!difficulty.has_modifier(Modifier::No_ability_upgrades)) {
		return true;
	}
	for (int i = 0; i < catalog_.get_count(); i++) {
		if (robots.find_ability(Robot_roster::kPlayerIndex, catalog_.get_name(i)) < 0) {
			return true;
		}
	}
	return false;
}

bool Rank_rewards::learn(Robot_roster& robots, shared_ptr<Robot_abilities> ability, const Difficulty& difficulty,
						 Event_reporter& reporter) {
	if (!is_available(robots, difficulty) || ability == nullptr
		|| robots.find_ability(Robot_roster::kPlayerIndex, ability->get_name()) >= 0) {
		return false;
	}
	robots.add_ability(Robot_roster::kPlayerIndex, ability);
	rewarded_rank_++;
	reporter.info("Rank up: learned " + ability->get_name());
	return true;
}

bool Rank_rewards::upgrade(Robot_roster& robots, int ability_index, const Difficulty& difficulty,
						   Event_reporter& reporter) {
	if (!is_available(robots, difficulty) || ability_index < 0
		|| ability_index >= robots.get_ability_count(Robot_roster::kPlayerIndex)) {
		return false;
	}
	if (difficulty.has_modifier(Modifier::No_ability_upgrades)) {
		reporter.error("Upgrades are disabled by difficulty");
		return false;
	}
	Robot_abilities& ability = robots.get_ability(Robot_roster::kPlayerIndex, ability_index);
	string description = ability.get_upgrade_description();
	ability.upgrade();
	rewarded_rank_++;
	reporter.info("Rank up: " + ability.get_name() + " " + description);
	return true;
}
