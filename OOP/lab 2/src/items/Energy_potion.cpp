#include "includes/Energy_potion.h"
#include "../game/includes/Game_session.h"

using namespace std;

const int kEnergyAmount = 10;

string Energy_potion::get_name() const {
	return "Energy potion";
}

string Energy_potion::get_description() const {
	return "+" + to_string(kEnergyAmount) + " energy";
}

bool Energy_potion::can_use(const Game_session& session, int user_index) const {
	const Robot& user = session.get_robot(user_index);
	// нет смысла пить при полной энергии
	return user.is_alive() && user.get_current_energy() < user.get_max_energy();
}

void Energy_potion::use(Game_session& session, int user_index) const {
	session.restore_energy(user_index, kEnergyAmount);
}
