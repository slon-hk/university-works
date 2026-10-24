#ifndef OOP_PROJECT_SHIELD_H
#define OOP_PROJECT_SHIELD_H

#include "Base_status.h"

// Щит: полностью поглощает удары, не превышающие amount_. Удар сильнее щита
// пробивает его — проходит разница, а щит исчезает.
class Shield : public Base_status {
private:
	int amount_; // прочность щита
	bool expired_;

public:
	explicit Shield(int amount);

	std::string get_name() const override;
	std::string get_description() const override;
	int get_amount() const override;
	std::shared_ptr<Robot_status> clone() const override;
	bool merge(const Robot_status& other) override;
	int absorb_damage(int damage) override;

	bool is_expired() const override;
};

#endif
