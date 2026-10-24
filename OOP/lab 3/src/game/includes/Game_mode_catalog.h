#ifndef OOP_PROJECT_GAME_MODE_CATALOG_H
#define OOP_PROJECT_GAME_MODE_CATALOG_H

#include <memory>
#include <string>

#include "Difficulty.h"
#include "Game.h"
#include "Game_event_listener.h"

// Каталог режимов игры: единственное место, где перечислены режимы
// (генератор уровней и их количество). Даёт названия для меню и создаёт игры.
class Game_mode_catalog {
public:
	int get_count() const;
	// название для меню и журнала, например "random levels"
	std::string get_title(int index) const;

	// пустая игра режима index (ещё не начатая); бросает out_of_range
	std::unique_ptr<Game> create(int index) const;
	// новая игра режима index с подключённым слушателем событий
	std::unique_ptr<Game> create_new(int index, const Difficulty& difficulty, Game_event_listener* listener) const;
	// пустая игра режима с именем mode (см. Game::get_mode_name), например
	// для восстановления из сохранения; бросает runtime_error, если такого нет
	std::unique_ptr<Game> create_by_name(const std::string& mode) const;
};

#endif
