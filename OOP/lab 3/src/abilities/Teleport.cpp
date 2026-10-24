#include "includes/Teleport.h"
#include "../game/includes/Game_session.h"

using namespace std;

const int kStartEnergyCost = 5;
const int kMinEnergyCost = 1;
const int kTeleportRadius = 5;

Teleport::Teleport() : energy_cost_(kStartEnergyCost), level_(1) {
}

string Teleport::get_name() const {
	return "Teleport";
}

size_t Teleport::get_energy_cost() const {
	return energy_cost_;
}

int Teleport::get_radius() const {
	return kTeleportRadius;
}

bool Teleport::needs_target() const {
	return true;
}

bool Teleport::can_use(const Game_session& session, int user_index, int target_x, int target_y) const {
	return session.get_robots().get_robot(user_index).is_alive()
		&& session.get_robots().is_in_radius(user_index, target_x, target_y, kTeleportRadius)
		&& session.is_cell_free(target_x, target_y);
}

void Teleport::use(Game_session &session, int user_index, int target_x, int target_y) const {
	session.move_robot(user_index, target_x, target_y);
}

int Teleport::get_level() const {
	return level_;
}

string Teleport::get_upgrade_description() const {
	if (energy_cost_ <= kMinEnergyCost) {
		return "energy " + to_string(energy_cost_) + " (minimum)";
	}
	return "energy " + to_string(energy_cost_) + " -> " + to_string(energy_cost_ - 1);
}

void Teleport::upgrade() {
	if (energy_cost_ > kMinEnergyCost) {
		energy_cost_--;
	}
	level_++;
}
