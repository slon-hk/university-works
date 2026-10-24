#include "includes/Player_state.h"

#include <stdexcept>

using namespace std;

// параметры: HP, макс. HP, атака, энергия, макс. энергия, опыт, опыт до ранга,
// ранг, обзор, сила лечения, скорость, отношение
Player_state::Player_state()
	: robot_(100, 100, 10, 10, 10, 0, 50, 1, 7, 10, 5, Relation::Neutral), inventory_(kInventoryCapacity) {
}

Player_state::Player_state(const Robot& robot, const vector<shared_ptr<Robot_abilities>>& abilities,
						   const Inventory& inventory)
	: robot_(robot), abilities_(abilities), inventory_(inventory) {
	if (robot_.get_relation() != Relation::Neutral) {
		throw invalid_argument("player robot must be neutral");
	}
	for (size_t i = 0; i < abilities_.size(); i++) {
		if (abilities_[i] == nullptr) {
			throw invalid_argument("player ability is missing");
		}
		for (size_t j = 0; j < i; j++) {
			if (abilities_[j] == abilities_[i]) {
				throw invalid_argument("player ability is repeated");
			}
		}
	}
	if (inventory_.get_capacity() != (int) kInventoryCapacity) {
		throw invalid_argument("player inventory has wrong capacity");
	}
}

const Robot& Player_state::get_robot() const {
	return robot_;
}

const vector<shared_ptr<Robot_abilities>>& Player_state::get_abilities() const {
	return abilities_;
}

const Inventory& Player_state::get_inventory() const {
	return inventory_;
}

void Player_state::prepare_for_next_level(bool keep_statuses) {
	if (!keep_statuses) {
		robot_.clear_statuses();
	}
	robot_.reset_energy();
	robot_.restore_movement_points();
}
