#ifndef OOP_PROJECT_GAME_EVENT_LISTENER_H
#define OOP_PROJECT_GAME_EVENT_LISTENER_H

#include <string>

#include "Game_event_type.h"

// Наблюдатель за событиями игры. Игра лишь сообщает о событиях,
// а что с ними делать (например, писать в файл), решает слушатель.
class Game_event_listener {
public:
	virtual ~Game_event_listener() = default;

	virtual void on_game_event(Game_event_type type, const std::string& text) = 0;
};

#endif
