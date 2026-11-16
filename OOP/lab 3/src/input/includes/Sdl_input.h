#ifndef OOP_PROJECT_SDL_INPUT_H
#define OOP_PROJECT_SDL_INPUT_H

#include <string>

#include "SDL.h"

#include "../../game/includes/Command.h"
#include "Key_bindings.h"

// Ввод с клавиатуры через события SDL
class Sdl_input {
private:
	Key_bindings bindings_;
	std::string warning_;

	std::string get_combo(const SDL_KeyboardEvent& key) const;

public:
	Sdl_input(const std::string& config_path);
	~Sdl_input();

	bool read_command(Command& command);

	const Key_labels& get_labels() const;
	// пусто, если файл управления корректен
	const std::string& get_warning() const;
};

#endif
