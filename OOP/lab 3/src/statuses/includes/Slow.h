#ifndef OOP_PROJECT_SLOW_H
#define OOP_PROJECT_SLOW_H

#include "Base_status.h"

// Замедление: запрещает перемещение (но не атаки) на turns_ ходов.
class Slow : public Base_status {
private:
	int turns_;

public:
	explicit Slow(int turns = 1);

	std::string get_name() const override;
	std::string get_description() const override;
	int get_amount() const override;
	std::shared_ptr<Robot_status> clone() const override;
	bool merge(const Robot_status& other) override;
	bool blocks_movement() const override;
	int on_turn_end() override;

	bool is_expired() const override;
};

#endif
