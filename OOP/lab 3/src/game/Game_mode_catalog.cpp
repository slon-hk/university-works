#include "includes/Game_mode_catalog.h"

#include <stdexcept>

#include "includes/File_level_generator.h"
#include "includes/Game_template.h"
#include "includes/Random_level_generator.h"

using namespace std;

const int kModeCount = 3;
const char* const kModeTitles[kModeCount] = { "fixed levels", "random levels", "endless mode" };

int Game_mode_catalog::get_count() const {
	return kModeCount;
}

string Game_mode_catalog::get_title(int index) const {
	if (index < 0 || index >= kModeCount) {
		throw out_of_range("game mode " + to_string(index));
	}
	return kModeTitles[index];
}

unique_ptr<Game> Game_mode_catalog::create(int index) const {
	if (index == 0) {
		return make_unique<Game_template<File_level_generator, 3>>();
	}
	if (index == 1) {
		return make_unique<Game_template<Random_level_generator, 5>>();
	}
	if (index == 2) {
		return make_unique<Game_template<Random_level_generator, kEndless_levels>>();
	}
	throw out_of_range("game mode " + to_string(index));
}

unique_ptr<Game> Game_mode_catalog::create_new(int index, const Difficulty& difficulty,
											   Game_event_listener* listener) const {
	unique_ptr<Game> game = create(index);
	// слушатель подключается до старта, чтобы попали события первого уровня
	game->set_event_listener(listener);
	game->start_new(difficulty);
	return game;
}

unique_ptr<Game> Game_mode_catalog::create_by_name(const string& mode) const {
	for (int i = 0; i < kModeCount; i++) {
		unique_ptr<Game> game = create(i);
		if (game->get_mode_name() == mode) {
			return game;
		}
	}
	throw runtime_error("unknown game mode in save: " + mode);
}
