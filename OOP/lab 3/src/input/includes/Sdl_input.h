#ifndef OOP_PROJECT_SDL_INPUT_H
#define OOP_PROJECT_SDL_INPUT_H

#include <string>

#include "SDL.h"

#include "../../game/includes/Command.h"
#include "Key_bindings.h"

// Ввод с клавиатуры через события SDL: нажатая клавиша по таблице
// Key_bindings превращается в игровую команду, закрытие окна — в Quit.
// Если файл управления некорректен, используется управление по умолчанию,
// а причина доступна через get_warning().
class Sdl_input {
private:
	Key_bindings bindings_;
	std::string warning_;

	// нажатие в виде комбинации для Key_bindings: "S", "Shift+S", ...
	std::string get_combo(const SDL_KeyboardEvent& key) const;

public:
	explicit Sdl_input(const std::string& config_path);
	~Sdl_input();

	// не ждёт ввода: true и команда, если в очереди есть нажатие
	// назначенной клавиши или закрытие окна, иначе false
	bool read_command(Command& command);

	const Key_labels& get_labels() const;
	// пусто, если файл управления корректен
	const std::string& get_warning() const;
};

#endif
