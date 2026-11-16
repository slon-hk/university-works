#ifndef OOP_PROJECT_KEY_BINDINGS_H
#define OOP_PROJECT_KEY_BINDINGS_H

#include <map>
#include <string>

#include "../../game/includes/Command.h"
#include "../../game/includes/Key_labels.h"

// Назначение клавиш командам.
// Формат файла: строка "<команда> <клавиша>", например "move_up W";
class Key_bindings {
private:
	std::map<std::string, Command> commands_;
	Key_labels keys_;

	void bind(Command command, const std::string& key);
	void parse_line(const std::string& line, int line_number);
	std::string to_upper(std::string text) const;
	std::string trim(const std::string& text) const;
	bool cut_modifier(std::string& combo, int& modifier) const;
	std::string normalize(const std::string& combo) const;

public:
	// управление по умолчанию
	Key_bindings();

	Key_bindings(const std::string& path);

	std::string get_command_name(Command command) const;

	std::string get_key_name(const std::string& combo) const;

	bool find_command(const std::string& key, Command& command) const;

	const Key_labels& get_labels() const;
};

#endif
