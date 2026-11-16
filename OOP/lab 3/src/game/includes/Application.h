#ifndef OOP_PROJECT_APPLICATION_H
#define OOP_PROJECT_APPLICATION_H

#include <memory>
#include <string>

#include "../../logging/includes/Game_log_listener.h"
#include "../../logging/includes/Logger.h"
#include "Command.h"
#include "Command_slots.h"
#include "Game.h"
#include "Game_mode_catalog.h"
#include "Save_slot.h"
#include "Screen.h"

// Верхний уровень игры: главное меню, запуск новой игры, сохранение
// и загрузка через Save_slot, возврат в меню
class Application {
private:
	Save_slot save_slot_;
	Logger& logger_;
	Game_log_listener game_listener_;
	Game_mode_catalog modes_;
	Command_slots slots_;
	std::unique_ptr<Game> game_;
	Screen screen_;
	std::string message_title_;
	std::string message_text_;
	std::string notification_;
	bool running_;
	int pending_mode_;

	void handle_menu(Command command);
	void handle_difficulty(Command command);
	void handle_game(Command command);

	void enter_game(std::unique_ptr<Game> game, const std::string& event);
	void save_game();

	bool load_game(std::string& error);
	void load_in_game();

	std::string describe_game() const;

public:
	Application(const std::string& save_path, Logger& logger);

	void handle(Command command);
	void show_message(const std::string& title, const std::string& text);

	bool is_running() const;
	Screen get_screen() const;
	bool has_save() const;
	int get_pending_mode() const;
	const std::string& get_message_title() const;
	// строки сообщения разделены '\n'
	const std::string& get_message_text() const;
	const std::string& get_notification() const;
	const Game& get_game() const;
};

#endif
