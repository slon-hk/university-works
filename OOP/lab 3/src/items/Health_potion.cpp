#include "includes/Health_potion.h"
#include "../game/includes/Game_session.h"

using namespace std;

const int kHealAmount = 40;

string Health_potion::get_name() const {
	return "Health potion";
}

string Health_potion::get_description() const {
	return "+" + to_string(kHealAmount) + " HP";
}

bool Health_potion::can_use(const Game_session& session, int user_index) const {
	const Robot& user = session.get_robots().get_robot(user_index);
	// нет смысла пить при полном здоровье
	return user.is_alive() && user.get_current_health() < user.get_max_health();
}

void Health_potion::use(Game_session& session, int user_index) const {
	session.heal_robot(user_index, kHealAmount);
}
