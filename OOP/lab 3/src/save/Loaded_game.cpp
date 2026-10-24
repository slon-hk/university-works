#include "includes/Loaded_game.h"

#include <stdexcept>

#include "../abilities/includes/Ability_catalog.h"

using namespace std;

Loaded_game::Loaded_game(const string& mode, int level, unique_ptr<Game_session> session, int selected_slot,
						 bool ability_switched)
	: mode_(mode), level_(level), session_(std::move(session)), selected_slot_(selected_slot),
	  ability_switched_(ability_switched) {
	if (mode_.empty()) {
		throw invalid_argument("game mode is empty");
	}
	if (level_ < 1) {
		throw invalid_argument("level must be positive");
	}
	if (session_ == nullptr) {
		throw invalid_argument("game session is missing");
	}
	if (selected_slot_ < 0 || selected_slot_ >= Ability_catalog().get_count()) {
		throw invalid_argument("selected ability slot is out of range");
	}
}

const string& Loaded_game::get_mode() const {
	return mode_;
}

int Loaded_game::get_level() const {
	return level_;
}

int Loaded_game::get_selected_slot() const {
	return selected_slot_;
}

bool Loaded_game::is_ability_switched() const {
	return ability_switched_;
}

unique_ptr<Game_session> Loaded_game::release_session() {
	if (session_ == nullptr) {
		throw logic_error("game session was already released");
	}
	return std::move(session_);
}
