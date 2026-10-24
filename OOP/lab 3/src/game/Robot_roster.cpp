#include "includes/Robot_roster.h"

#include <cstdlib>

using namespace std;

Robot_unit& Robot_roster::unit(int index) {
	return units_.at(index);
}

const Robot_unit& Robot_roster::unit(int index) const {
	return units_.at(index);
}

void Robot_roster::add(const Robot& robot, int x, int y, const vector<shared_ptr<Robot_abilities>>& abilities) {
	units_.push_back(Robot_unit(robot, x, y, abilities));
}

int Robot_roster::get_count() const {
	return (int) units_.size();
}

bool Robot_roster::is_empty() const {
	return units_.empty();
}

bool Robot_roster::is_player(int index) const {
	return index == kPlayerIndex;
}

string Robot_roster::get_name(int index) const {
	if (is_player(index)) {
		return "Player";
	}
	if (get_robot(index).get_relation() == Relation::Enemy) {
		return "Enemy #" + to_string(index);
	}
	return "Ally #" + to_string(index);
}

Robot& Robot_roster::get_robot(int index) {
	return unit(index).get_robot();
}

const Robot& Robot_roster::get_robot(int index) const {
	return unit(index).get_robot();
}

int Robot_roster::get_x(int index) const {
	return unit(index).get_x();
}

int Robot_roster::get_y(int index) const {
	return unit(index).get_y();
}

void Robot_roster::move_to(int index, int x, int y) {
	unit(index).move_to(x, y);
}

int Robot_roster::find_at(int x, int y) const {
	for (size_t i = 0; i < units_.size(); i++) {
		if (units_[i].occupies(x, y)) {
			return (int) i;
		}
	}
	return -1;
}

bool Robot_roster::is_in_radius(int index, int x, int y, int radius) const {
	return abs(x - get_x(index)) <= radius && abs(y - get_y(index)) <= radius;
}

bool Robot_roster::has_living_enemies() const {
	for (size_t i = 0; i < units_.size(); i++) {
		const Robot& robot = units_[i].get_robot();
		if (robot.is_alive() && robot.get_relation() == Relation::Enemy) {
			return true;
		}
	}
	return false;
}

int Robot_roster::get_ability_count(int index) const {
	return (int) unit(index).get_abilities().size();
}

const Robot_abilities& Robot_roster::get_ability(int robot_index, int ability_index) const {
	return *share_ability(robot_index, ability_index);
}

Robot_abilities& Robot_roster::get_ability(int robot_index, int ability_index) {
	return *share_ability(robot_index, ability_index);
}

shared_ptr<Robot_abilities> Robot_roster::share_ability(int robot_index, int ability_index) const {
	return unit(robot_index).get_abilities().at(ability_index);
}

int Robot_roster::find_ability(int robot_index, const string& name) const {
	const vector<shared_ptr<Robot_abilities>>& abilities = get_abilities(robot_index);
	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilities[i]->get_name() == name) {
			return (int) i;
		}
	}
	return -1;
}

const vector<shared_ptr<Robot_abilities>>& Robot_roster::get_abilities(int index) const {
	return unit(index).get_abilities();
}

void Robot_roster::set_abilities(int index, const vector<shared_ptr<Robot_abilities>>& abilities) {
	unit(index).set_abilities(abilities);
}

void Robot_roster::add_ability(int index, shared_ptr<Robot_abilities> ability) {
	unit(index).add_ability(ability);
}
