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
// не считывает, не рисует и не пишет в файлы.
class Game {
public:
	virtual ~Game() = default;

	// новая игра с первого уровня; бросает runtime_error, если уровень
	// не удалось создать
	virtual void start_new(const Difficulty& difficulty) = 0;
	virtual void handle(Command command) = 0;

	virtual Game_phase get_phase() const = 0;
	// номер текущего уровня (с 1) и общее количество (0 — бесконечный режим)
	virtual int get_level() const = 0;
	virtual int get_level_count() const = 0;
	// текущий уровень и состояние ввода игрока (после начала игры)
	virtual const Game_session& get_session() const = 0;
	virtual const Player_controller& get_controller() const = 0;
	virtual const Display_mode_selector& get_display_mode() const = 0;

	// слушатель игровых событий для всех уровней (не владеет им)
	virtual void set_event_listener(Game_event_listener* listener) = 0;

	// имя режима (генератор уровней и их количество), например "random_5"
	virtual std::string get_mode_name() const = 0;
	// может ли игрок сейчас действовать — только тогда разрешены сохранение и загрузка
	virtual bool can_act() const = 0;
	// заменяет текущее состояние восстановленным (для системы загрузки).
	// Бросает runtime_error, если уровень вне диапазона режима или
	// в таком состоянии игрок не может действовать; тогда игра не меняется.
	virtual void restore(int level, std::unique_ptr<Game_session> session, int selected_slot,
						 bool ability_switched) = 0;
};

#endif
