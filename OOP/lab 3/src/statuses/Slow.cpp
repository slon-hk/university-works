#include "includes/Slow.h"

using namespace std;

Slow::Slow(int turns) : turns_(turns > 0 ? turns : 1) {
}

string Slow::get_name() const {
	return "Slow";
}

string Slow::get_description() const {
	return "Slow(" + to_string(turns_) + ")";
}

int Slow::get_amount() const {
	return turns_;
}

shared_ptr<Robot_status> Slow::clone() const {
	return make_shared<Slow>(*this);
}

bool Slow::merge(const Robot_status& other) {
	if (other.get_name() != get_name()) {
		return false;
	}
	turns_ += other.get_amount();
	return true;
}

bool Slow::blocks_movement() const {
	return true;
}

int Slow::on_turn_end() {
	turns_--;
	return 0;
}

bool Slow::is_expired() const {
	return turns_ <= 0;
}
