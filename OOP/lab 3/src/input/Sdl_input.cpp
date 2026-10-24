#include "includes/Sdl_input.h"

#include "SDL.h"

#include <iostream>
#include <stdexcept>

using namespace std;

Sdl_input::Sdl_input(const string& config_path) {
	SDL_InitSubSystem(SDL_INIT_EVENTS);
	try {
		Key_bindings loaded(config_path);
		// названия клавиш проверяет SDL
		for (const auto& binding : loaded.get_labels().get_all()) {
			if (SDL_GetScancodeFromName(loaded.get_key_name(binding.second).c_str()) == SDL_SCANCODE_UNKNOWN) {
				throw runtime_error("unknown key '" + binding.second + "' for command '"
									+ loaded.get_command_name(binding.first) + "'");
			}
		}
		bindings_ = loaded;
	} catch (const exception& error) {
		warning_ = string("Controls file is invalid: ") + error.what() + "\nDefault controls are used";
		cerr << warning_ << endl;
	}
}

Sdl_input::~Sdl_input() {
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

bool Sdl_input::read_command(Command& command) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			command = Command::Quit;
			return true;
		}
		if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat
			&& bindings_.find_command(get_combo(event.key), command)) {
			return true;
		}
	}
	return false;
}

string Sdl_input::get_combo(const SDL_KeyboardEvent& key) const {
	string combo;
	// сама клавиша-модификатор (Left Shift и т.п.) нажимается без префиксов
	if (key.scancode < SDL_SCANCODE_LCTRL || key.scancode > SDL_SCANCODE_RGUI) {
		if (key.mod & SDL_KMOD_CTRL) {
			combo += "Ctrl+";
		}
		if (key.mod & SDL_KMOD_SHIFT) {
			combo += "Shift+";
		}
		if (key.mod & SDL_KMOD_ALT) {
			combo += "Alt+";
		}
		if (key.mod & SDL_KMOD_GUI) {
			combo += "Cmd+";
		}
	}
	return combo + SDL_GetScancodeName(key.scancode);
}

const Key_labels& Sdl_input::get_labels() const {
	return bindings_.get_labels();
}

const string& Sdl_input::get_warning() const {
	return warning_;
}
