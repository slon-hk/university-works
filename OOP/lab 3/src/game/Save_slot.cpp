#include "includes/Save_slot.h"

using namespace std;

Save_slot::Save_slot(const string& path) : path_(path) {
}

const string& Save_slot::get_path() const {
	return path_;
}

bool Save_slot::exists() const {
	return serializer_.exists(path_);
}

void Save_slot::save(const Game& game) const {
	serializer_.save(game, path_);
}

unique_ptr<Game> Save_slot::load(Game_event_listener* listener) const {
	Loaded_game loaded = serializer_.load(path_);
	unique_ptr<Game> game = modes_.create_by_name(loaded.get_mode());
	game->set_event_listener(listener);
	game->restore(loaded.get_level(), loaded.release_session(), loaded.get_selected_slot(),
				  loaded.is_ability_switched());
	return game;
}
