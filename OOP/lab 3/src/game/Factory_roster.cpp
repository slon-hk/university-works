#include "includes/Factory_roster.h"

#include <stdexcept>
#include <string>

using namespace std;

void Factory_roster::add(const Robot_factory& factory, int x, int y) {
	factories_.push_back(Placed_factory(factory, x, y));
}

void Factory_roster::remove(int index) {
	if (index < 0 || index >= get_count()) {
		throw out_of_range("factory " + to_string(index));
	}
	factories_.erase(factories_.begin() + index);
}

int Factory_roster::get_count() const {
	return (int) factories_.size();
}

Robot_factory& Factory_roster::get_factory(int index) {
	return factories_.at(index).get_factory();
}

const Robot_factory& Factory_roster::get_factory(int index) const {
	return factories_.at(index).get_factory();
}

int Factory_roster::get_x(int index) const {
	return factories_.at(index).get_x();
}

int Factory_roster::get_y(int index) const {
	return factories_.at(index).get_y();
}

int Factory_roster::find_at(int x, int y) const {
	for (size_t i = 0; i < factories_.size(); i++) {
		if (factories_[i].covers(x, y)) {
			return (int) i;
		}
	}
	return -1;
}

bool Factory_roster::has_enemy_factories() const {
	for (size_t i = 0; i < factories_.size(); i++) {
		if (factories_[i].get_factory().get_unit().get_relation() == Relation::Enemy) {
			return true;
		}
	}
	return false;
}
