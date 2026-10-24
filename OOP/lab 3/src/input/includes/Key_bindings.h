#ifndef OOP_PROJECT_KEY_BINDINGS_H
#define OOP_PROJECT_KEY_BINDINGS_H

#include <map>
#include <string>

#include "../../game/includes/Command.h"
#include "../../game/includes/Key_labels.h"

// Назначение клавиш командам. Клавиша задаётся названием (например "W",
// "Return", "Left Shift"); регистр не важен. Перед клавишей можно указать
// модификаторы через '+': "Shift+S", "Ctrl+Shift+F5" (Ctrl, Shift, Alt, Cmd). Каждой команде, кроме Quit,
// назначена ровно одна клавиша, и каждая клавиша отвечает за одну команду.
//
// Формат файла: строка "<команда> <клавиша>", например "move_up W";
// пустые строки и строки, начинающиеся с '#', пропускаются.
class Key_bindings {
private:
	std::map<std::string, Command> commands_; // нормализованная комбинация -> команда
	Key_labels keys_; // команда -> клавиша (как в файле)

	// назначает клавишу; бросает runtime_error при повторах
	void bind(Command command, const std::string& key);
	// разбирает строку файла "<команда> <клавиша>"
	void parse_line(const std::string& line, int line_number);
	std::string to_upper(std::string text) const;
	std::string trim(const std::string& text) const;
	// отрезает модификатор в начале комбинации ("Shift+S" -> "S"); false, если его нет
	bool cut_modifier(std::string& combo, int& modifier) const;
	// единая запись комбинации: верхний регистр, модификаторы в фиксированном порядке
	std::string normalize(const std::string& combo) const;

public:
	// управление по умолчанию
	Key_bindings();
	// читает и проверяет файл; бросает runtime_error с описанием ошибки,
	// если файла нет, команда неизвестна, на команду не назначена клавиша,
	// на команду назначено несколько клавиш или клавиша назначена на несколько команд
	explicit Key_bindings(const std::string& path);

	// имя команды в файле: "move_up", "ability_1", ...
	std::string get_command_name(Command command) const;

	// название клавиши без модификаторов: "Shift+S" -> "S"
	std::string get_key_name(const std::string& combo) const;

	// команда для комбинации ("S", "Shift+S"); false, если она ни на что не назначена
	bool find_command(const std::string& key, Command& command) const;
	// команда -> название клавиши (для подсказок и проверки клавиш)
	const Key_labels& get_labels() const;
};

#endif
