#include "includes/Around_attack.h"
#include "../game/includes/Game_session.h"
#include "../statuses/includes/Overload.h"

using namespace std;

const int kEnergyCost = 4;
const int kStartRadius = 1;
const int kRadiusPerLevel = 1;

Around_attack::Around_attack() : radius_(kStartRadius), level_(1) {
}

string Around_attack::get_name() const {
	return "Around attack";
}

size_t Around_attack::get_energy_cost() const {
	return kEnergyCost;
}

int Around_attack::get_radius() const {
	return radius_;
}

bool Around_attack::needs_target() const {
	return false;
}

bool Around_attack::can_use(const Game_session& session, int user_index, int target_x, int target_y) const {
	return session.get_robots().get_robot(user_index).is_alive() && !find_targets(session, user_index).empty();
}

void Around_attack::use(Game_session& session, int user_index, int target_x, int target_y) const {
	vector<int> targets = find_targets(session, user_index);
	int attack_power = (int) session.get_robots().get_robot(user_index).get_attack_power();
	for (size_t i = 0; i < targets.size(); i++) {
		session.deal_damage(user_index, targets[i], attack_power);
		session.apply_status(targets[i], make_shared<Overload>());
	}
}

vector<int> Around_attack::find_targets(const Game_session& session, int user_index) const {
	vector<int> targets;
	int user_x = session.get_robots().get_x(user_index);
	int user_y = session.get_robots().get_y(user_index);
	// перебираем квадрат (2r+1)x(2r+1) с центром в атакующем;
	// бьёт без разбора — и врагов, и союзников
	for (int y = user_y - radius_; y <= user_y + radius_; y++) {
		for (int x = user_x - radius_; x <= user_x + radius_; x++) {
			int index = session.get_robots().find_at(x, y);
			if (index >= 0 && index != user_index) {
				targets.push_back(index);
			}
		}
	}
	return targets;
}

int Around_attack::get_level() const {
	return level_;
}

string Around_attack::get_upgrade_description() const {
	return "radius " + to_string(radius_) + " -> " + to_string(radius_ + kRadiusPerLevel);
}

void Around_attack::upgrade() {
	radius_ += kRadiusPerLevel;
	level_++;
}
