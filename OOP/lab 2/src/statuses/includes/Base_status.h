#ifndef OOP_PROJECT_BASE_STATUS_H
#define OOP_PROJECT_BASE_STATUS_H

#include "Robot_status.h"

// Базовая реализация статуса с нейтральным поведением всех хуков
// (ничего не блокирует, урон не меняет, не сливается).
class Base_status : public Robot_status {
public:
	int get_amount() const override;
	bool merge(const Robot_status& other) override;

	bool blocks_movement() const override;
	bool blocks_ability() const override;

	void on_ability_attempt() override;
	int absorb_damage(int damage) override;
	int on_turn_end() override;

	void on_apply(Game_session& session, int robot_index) override;
	int get_attack_bonus() const override;
};

#endif
