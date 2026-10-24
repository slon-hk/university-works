#ifndef OOP_PROJECT_LONG_ATTACK_H
#define OOP_PROJECT_LONG_ATTACK_H

#include "Robot_abilities.h"

// Дальняя атака по выбранной клетке в радиусе kAttackDistance: урон равен
// атаке робота плюс бонус, цель поджигается (Burn). Прокачка увеличивает бонус.
class Long_attack : public Robot_abilities {
private:
	int bonus_damage_;
	int level_;

public:
	Long_attack();

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

#endif //OOP_PROJECT_LONG_ATTACK_H
