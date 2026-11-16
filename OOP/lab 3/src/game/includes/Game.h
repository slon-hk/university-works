#ifndef OOP_PROJECT_GAME_H
#define OOP_PROJECT_GAME_H

#include <memory>
#include <string>

#include "Command.h"
#include "Difficulty.h"
#include "Display_mode_selector.h"
#include "Game_event_listener.h"
#include "Game_phase.h"
#include "Game_session.h"
#include "Player_controller.h"

// Интерфейс игры для приложения, систем ввода, визуализации и сохранения:
// игра только принимает команды и отдаёт своё состояние, сама ничего
class Game {
public:
	virtual ~Game() = default;

	virtual void start_new(const Difficulty& difficulty) = 0;
	virtual void handle(Command command) = 0;

	virtual Game_phase get_phase() const = 0;

	virtual int get_level() const = 0;
	virtual int get_level_count() const = 0;

	virtual const Game_session& get_session() const = 0;
	virtual const Player_controller& get_controller() const = 0;
	virtual const Display_mode_selector& get_display_mode() const = 0;

	virtual void set_event_listener(Game_event_listener* listener) = 0;

	virtual std::string get_mode_name() const = 0;

	virtual bool can_act() const = 0;
	// заменяет текущее состояние восстановленным (для системы загрузки).
	virtual void restore(int level, std::unique_ptr<Game_session> session, int selected_slot,
						 bool ability_switched) = 0;
};

#endif
