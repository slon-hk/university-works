#include "includes/Attack_potion.h"
#include "../game/includes/Game_session.h"
#include "../statuses/includes/Strength.h"

using namespace std;

// бонус к атаке и его длительность в ходах
const int kAttackBonus = 10;
const int kBoostTurns = 3;

string Attack_potion::get_name() const {
	return "Attack potion";
}

string Attack_potion::get_description() const {
	return "+" + to_string(kAttackBonus) + " attack for " + to_string(kBoostTurns) + " turns";
}

bool Attack_potion::can_use(const Game_session& session, int user_index) const {
	return session.get_robots().get_robot(user_index).is_alive();
}

void Attack_potion::use(Game_session& session, int user_index) const {
	session.apply_status(user_index, make_shared<Strength>(kBoostTurns, kAttackBonus));
}
