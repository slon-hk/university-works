#ifndef OOP_PROJECT_RANK_REWARDS_H
#define OOP_PROJECT_RANK_REWARDS_H

#include <memory>

#include "../../abilities/includes/Ability_catalog.h"
#include "../../abilities/includes/Robot_abilities.h"
#include "Difficulty.h"
#include "Event_reporter.h"
#include "Robot_roster.h"

// Награды за ранг: на каждый новый ранг игрок может выучить новую
// способность или улучшить имеющуюся (с модификатором No_ability_upgrades —
// только выучить; когда учить нечего, награда пропускается).
// Хранит, до какого ранга награды уже выданы.
class Rank_rewards {
private:
	Ability_catalog catalog_;
	int rewarded_rank_;

	// можно ли за ранг выбрать хоть что-то (с учётом No_ability_upgrades)
	bool has_choice(const Robot_roster& robots, const Difficulty& difficulty) const;

public:
	Rank_rewards();

	int get_rewarded_rank() const;
	// ранг, до которого награды считаются выданными
	void set_rewarded_rank(int rank);

	// игрок жив, его ранг выше награждённого и выбрать есть что
	bool is_available(const Robot_roster& robots, const Difficulty& difficulty) const;
	bool learn(Robot_roster& robots, std::shared_ptr<Robot_abilities> ability, const Difficulty& difficulty,
			   Event_reporter& reporter);
	bool upgrade(Robot_roster& robots, int ability_index, const Difficulty& difficulty, Event_reporter& reporter);
};

#endif
