#ifndef OOP_PROJECT_STRENGTH_H
#define OOP_PROJECT_STRENGTH_H

#include "Base_status.h"

// Усиление: даёт +bonus_ к атаке на turns_ ходов (накладывается зельем атаки).
class Strength : public Base_status {
private:
	int turns_;
	int bonus_;

public:
	Strength(int turns, int bonus);

	std::string get_name() const override;
	std::string get_description() const override;
	int get_amount() const override;
	std::shared_ptr<Robot_status> clone() const override;
	bool merge(const Robot_status& other) override;
	int on_turn_end() override;
	int get_attack_bonus() const override;

	bool is_expired() const override;
};

#endif
