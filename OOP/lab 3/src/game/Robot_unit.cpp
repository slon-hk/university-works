#include "includes/Robot_unit.h"

using namespace std;

Robot_unit::Robot_unit(const Robot& robot, int x, int y, const vector<shared_ptr<Robot_abilities>>& abilities)
	: robot_(robot), x_(x), y_(y) {
	set_abilities(abilities);
}

Robot& Robot_unit::get_robot() {
	return robot_;
}

const Robot& Robot_unit::get_robot() const {
	return robot_;
}

int Robot_unit::get_x() const {
	return x_;
}

int Robot_unit::get_y() const {
	return y_;
}

void Robot_unit::move_to(int x, int y) {
	x_ = x;
	y_ = y;
}

bool Robot_unit::occupies(int x, int y) const {
	return robot_.is_alive() && x_ == x && y_ == y;
}

const vector<shared_ptr<Robot_abilities>>& Robot_unit::get_abilities() const {
	return abilities_;
}

void Robot_unit::set_abilities(const vector<shared_ptr<Robot_abilities>>& abilities) {
	abilities_.clear();
	for (size_t i = 0; i < abilities.size(); i++) {
		add_ability(abilities[i]);
	}
}

void Robot_unit::add_ability(shared_ptr<Robot_abilities> ability) {
	if (ability != nullptr) {
		abilities_.push_back(ability);
	}
}
