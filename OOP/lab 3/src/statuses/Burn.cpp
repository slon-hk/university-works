#include "includes/Burn.h"

using namespace std;

Burn::Burn(int turns, int damage) : turns_(turns > 0 ? turns : 1), damage_(damage > 0 ? damage : 0) {
}

string Burn::get_name() const {
	return "Burn";
}

string Burn::get_description() const {
	return "Burn(" + to_string(turns_) + ")";
}

int Burn::get_amount() const {
	return turns_;
}

int Burn::get_damage() const {
	return damage_;
}

shared_ptr<Robot_status> Burn::clone() const {
	return make_shared<Burn>(*this);
}

bool Burn::merge(const Robot_status& other) {
	if (other.get_name() != get_name()) {
		return false;
	}
	// горение продлевается на длительность нового статуса
	turns_ += other.get_amount();
	return true;
}

int Burn::on_turn_end() {
	if (turns_ <= 0) {
		return 0;
	}
	turns_--;
	return damage_;
}

bool Burn::is_expired() const {
	return turns_ <= 0;
}
