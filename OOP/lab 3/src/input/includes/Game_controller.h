#ifndef OOP_PROJECT_GAME_CONTROLLER_H
#define OOP_PROJECT_GAME_CONTROLLER_H

#include "../../game/includes/Application.h"
#include "../../game/includes/Command.h"

// Связывает систему ввода с игрой: забирает у Input готовые команды
// и только передаёт их приложению, ничего не зная о том, как они получены.
// Input должен иметь метод bool read_command(Command& command), который
// не ждёт ввода и возвращает false, если команд нет.
template <typename Input>
class Game_controller {
private:
	Input& input_;
	Application& application_;

public:
	Game_controller(Input& input, Application& application) : input_(input), application_(application) {
	}

	// передаёт приложению все накопившиеся команды
	void process_input() {
		Command command;
		while (application_.is_running() && input_.read_command(command)) {
			application_.handle(command);
		}
	}
};

#endif
