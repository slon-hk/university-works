#include "includes/Strength.h"

using namespace std;

Strength::Strength(int turns, int bonus) : turns_(turns > 0 ? turns : 1), bonus_(bonus > 0 ? bonus : 0) {
}

string Strength::get_name() const {
	return "Strength";
}

string Strength::get_description() const {
	return "Strength(+" + to_string(bonus_) + ", " + to_string(turns_) + ")";
}

int Strength::get_amount() const {
	return turns_;
}

shared_ptr<Robot_status> Strength::clone() const {
	return make_shared<Strength>(*this);
}

bool Strength::merge(const Robot_status& other) {
	if (other.get_name() != get_name()) {
		return false;
	}
	turns_ += other.get_amount();
	return true;
}

int Strength::on_turn_end() {
	turns_--;
	return 0;
}

int Strength::get_attack_bonus() const {
	return bonus_;
}

bool Strength::is_expired() const {
	return turns_ <= 0;
}
