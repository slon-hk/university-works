#ifndef OOP_PROJECT_TELEPORT_H
#define OOP_PROJECT_TELEPORT_H

#include "Robot_abilities.h"

// Телепорт в свободную клетку в радиусе kTeleportRadius.
// Прокачка снижает стоимость в энергии (не ниже kMinEnergyCost).
class Teleport : public Robot_abilities {
private:
	int energy_cost_;
	int level_;

public:
	Teleport();

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

#endif //OOP_PROJECT_TELEPORT_H
