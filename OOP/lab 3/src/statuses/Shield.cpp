#include "includes/Shield.h"

using namespace std;

Shield::Shield(int amount) : amount_(amount > 0 ? amount : 0), expired_(false) {
}

string Shield::get_name() const {
	return "Shield";
}

string Shield::get_description() const {
	return "Shield(" + to_string(amount_) + ")";
}

int Shield::get_amount() const {
	return amount_;
}

shared_ptr<Robot_status> Shield::clone() const {
	return make_shared<Shield>(*this);
}

bool Shield::merge(const Robot_status& other) {
	if (other.get_name() != get_name()) {
		return false;
	}
	amount_ += other.get_amount();
	return true;
}

int Shield::absorb_damage(int damage) {
	if (expired_ || damage <= 0) {
		return damage;
	}
	// слабый удар поглощается целиком, щит при этом не тратится
	if (damage <= amount_) {
		return 0;
	}
	// щит пробит: дальше проходит только излишек урона
	expired_ = true;
	return damage - amount_;
}

bool Shield::is_expired() const {
	return expired_;
}
