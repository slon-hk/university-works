#ifndef OOP_PROJECT_LOADED_GAME_H
#define OOP_PROJECT_LOADED_GAME_H

#include <memory>
#include <string>

#include "../../game/includes/Game_session.h"

// Состояние игры, прочитанное из файла и прошедшее проверку.
// Инвариант: режим не пуст, уровень >= 1, выбранный слот способности
// существует, сессия есть (пока её не забрали через release_session).
class Loaded_game {
private:
	std::string mode_; // см. Game::get_mode_name()
	int level_;
	std::unique_ptr<Game_session> session_;
	int selected_slot_;
	bool ability_switched_;

public:
	// бросает invalid_argument при нарушении инварианта
	Loaded_game(const std::string& mode, int level, std::unique_ptr<Game_session> session, int selected_slot,
				bool ability_switched);

	const std::string& get_mode() const;
	int get_level() const;
	int get_selected_slot() const;
	bool is_ability_switched() const;
	// передаёт сессию вызывающему; повторный вызов бросает logic_error
	std::unique_ptr<Game_session> release_session();
};

#endif
