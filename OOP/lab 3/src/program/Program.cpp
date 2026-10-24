#include "includes/Program.h"

#include "SDL.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "../game/includes/Application.h"
#include "../gui/includes/Game_view.h"
#include "../gui/includes/Sdl_renderer.h"
#include "../input/includes/Game_controller.h"
#include "../input/includes/Sdl_input.h"

using namespace std;

Program::Program(int argc, char* argv[]) : options_(argc, argv) {
	srand((unsigned) time(nullptr));
	if (!options_.get_error().empty()) {
		cerr << "Bad argument: " << options_.get_error() << ", all log levels are used" << endl;
	}
}

void Program::run() {
	{
		// LOG_DIR задаётся в CMakeLists.txt
		Program_logger logger(options_.get_log_settings(), LOG_DIR);
		logger.log(Log_level::Program_info,
				   "Program started, log levels: " + options_.get_log_settings().to_string());
		if (!options_.get_error().empty()) {
			logger.log(Log_level::Program_error, "Bad command line: " + options_.get_error());
		}
		run_loop(logger);
		logger.log(Log_level::Program_info, "Program finished");
	}
	// SDL закрывается, когда окно и шрифты уже освобождены
	SDL_Quit();
}

void Program::run_loop(Program_logger& logger) {
	// CONTROLS_PATH и SAVE_PATH задаются в CMakeLists.txt
	Sdl_input input(CONTROLS_PATH);
	if (input.get_warning().empty()) {
		logger.log(Log_level::Program_info, string("Controls loaded from ") + CONTROLS_PATH);
	} else {
		logger.log(Log_level::Program_error, input.get_warning());
	}
	// визуализация получает только подписи клавиш, а не саму систему ввода
	Sdl_renderer renderer(input.get_labels());
	Application application(SAVE_PATH, logger);
	if (!input.get_warning().empty()) {
		application.show_message("Controls", input.get_warning());
	}

	Game_controller<Sdl_input> controller(input, application);
	Game_view<Sdl_renderer> view(renderer, application);
	while (application.is_running()) {
		controller.process_input();
		view.draw();
	}
}
