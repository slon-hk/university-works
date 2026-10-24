#ifndef OOP_PROJECT_BURN_H
#define OOP_PROJECT_BURN_H

#include "Base_status.h"

// Горение: в конце каждого хода наносит damage_ урона, пока не кончатся ходы.
// При повторном наложении длительность суммируется.
class Burn : public Base_status {
private:
	int turns_; // сколько ходов ещё будет гореть
	int damage_; // урон за ход

public:
	static constexpr int kDefaultTurns = 3;
	static constexpr int kDefaultDamage = 5;

	explicit Burn(int turns = kDefaultTurns, int damage = kDefaultDamage);

	std::string get_name() const override;
	std::string get_description() const override;
	int get_amount() const override;
	int get_damage() const;
	std::shared_ptr<Robot_status> clone() const override;
	bool merge(const Robot_status& other) override;
	int on_turn_end() override;

	bool is_expired() const override;
};

#endif
