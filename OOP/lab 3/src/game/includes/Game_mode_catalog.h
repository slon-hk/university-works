#ifndef OOP_PROJECT_GAME_MODE_CATALOG_H
#define OOP_PROJECT_GAME_MODE_CATALOG_H

#include <memory>
#include <string>

#include "Difficulty.h"
#include "Game.h"
#include "Game_event_listener.h"

// Каталог режимов игры
class Game_mode_catalog {
public:
	int get_count() const;

	std::string get_title(int index) const;

	std::unique_ptr<Game> create(int index) const;
	std::unique_ptr<Game> create_new(int index, const Difficulty& difficulty, Game_event_listener* listener) const;

	std::unique_ptr<Game> create_by_name(const std::string& mode) const;
};

#endif
