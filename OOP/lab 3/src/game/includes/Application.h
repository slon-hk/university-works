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
// и загрузка через Save_slot, возврат в меню. Как и Game, только
// принимает команды и отдаёт состояние. Действия программы и их ошибки
// пишутся в Logger, события игры пересылаются туда же через Game_log_listener.
// В меню команды-слоты Ability_1.. выбирают режим новой игры из
// Game_mode_catalog, следующий за режимами слот или Load загружают
// сохранение, Cancel — выход. Затем слоты выбирают сложность
// (Easy, Normal, Hard), Cancel возвращает в меню. В игре Save и Load работают, когда игрок
// может действовать; неудачная загрузка оставляет игру как была.
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
	std::string notification_; // строка-оповещение поверх игры
	bool running_;
	int pending_mode_; // режим, для которого выбирается сложность

	void handle_menu(Command command);
	void handle_difficulty(Command command);
	void handle_game(Command command);
	// делает game текущей игрой и пишет в журнал event с её описанием
	void enter_game(std::unique_ptr<Game> game, const std::string& event);
	void save_game();
	// общий сценарий загрузки из меню и из игры: при успехе заменяет
	// текущую игру и возвращает true; иначе игра остаётся прежней,
	// а error — описание ошибки
	bool load_game(std::string& error);
	void load_in_game();
	// описание текущей игры для журнала: режим, уровень, сложность
	std::string describe_game() const;

public:
	Application(const std::string& save_path, Logger& logger);

	void handle(Command command);
	// показывает сообщение; после любой команды — снова меню
	void show_message(const std::string& title, const std::string& text);

	bool is_running() const;
	Screen get_screen() const;
	bool has_save() const;
	// режим (номер в Game_mode_catalog), для которого выбирается сложность
	int get_pending_mode() const;
	const std::string& get_message_title() const;
	// строки сообщения разделены '\n'
	const std::string& get_message_text() const;
	// результат последнего сохранения/загрузки в игре или пустая строка
	const std::string& get_notification() const;
	// текущая игра; бросает logic_error, если экран не Game
	const Game& get_game() const;
};

#endif
