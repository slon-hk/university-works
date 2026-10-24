#ifndef OOP_PROJECT_ARROUND_ATTACK_H
#define OOP_PROJECT_ARROUND_ATTACK_H

#include <vector>

#include "Robot_abilities.h"

// Круговая атака: бьёт всех роботов в квадрате radius_ вокруг себя
// и накладывает на них Overload. Прокачка увеличивает радиус.
class Around_attack : public Robot_abilities {
private:
	int radius_;
	int level_;

	// индексы всех роботов в радиусе, кроме самого атакующего
	std::vector<int> find_targets(const Game_session& session, int user_index) const;

public:
	Around_attack();

	std::string get_name() const override;
	std::size_t get_energy_cost() const override;
	int get_radius() const override;
	bool needs_target() const override;

	bool can_use(const Game_session &session, int user_index, int target_x, int target_y) const override;
	void use(Game_session &session, int user_index, int target_x, int target_y) const override;

	int get_level() const override;
	std::string get_upgrade_description() const override;
	void upgrade() override;
};

#endif
