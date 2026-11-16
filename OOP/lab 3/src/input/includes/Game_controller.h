#ifndef OOP_PROJECT_GAME_CONTROLLER_H
#define OOP_PROJECT_GAME_CONTROLLER_H

#include "../../game/includes/Application.h"
#include "../../game/includes/Command.h"


template <typename Input>
class Game_controller {
private:
	Input& input_;
	Application& application_;

public:
	Game_controller(Input& input, Application& application) : input_(input), application_(application) {
	}

	void process_input() {
		Command command;
		while (application_.is_running() && input_.read_command(command)) {
			application_.handle(command);
		}
	}
};

#endif
