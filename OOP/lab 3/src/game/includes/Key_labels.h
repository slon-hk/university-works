#ifndef OOP_PROJECT_KEY_LABELS_H
#define OOP_PROJECT_KEY_LABELS_H

#include <map>
#include <string>

#include "Command.h"
#include "Command_slots.h"

// Подписи клавиш для подсказок на экране: какая клавиша назначена на команду.
// Хранит только данные, поэтому визуализация может пользоваться ими,
// ничего не зная о системе ввода.
class Key_labels {
private:
	std::map<Command, std::string> labels_;
	Command_slots slots_;

public:
	void set(Command command, const std::string& key);
	bool has(Command command) const;
	// бросает out_of_range, если на команду ничего не назначено
	const std::string& get(Command command) const;
	const std::string& get_ability(int slot) const;
	const std::string& get_item(int slot) const;
	const std::map<Command, std::string>& get_all() const;
};

#endif
