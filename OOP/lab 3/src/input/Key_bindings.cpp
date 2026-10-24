#include "includes/Key_bindings.h"

#include <cctype>
#include <cstring>
#include <fstream>
#include <stdexcept>

using namespace std;

const char* const kCommandNames[kBindableCommandCount] = {
	"move_up", "move_down", "move_left", "move_right", "use_ability", "cancel", "end_turn",
	"ability_1", "ability_2", "ability_3", "ability_4",
	"item_1", "item_2", "item_3", "item_4", "item_5", "item_6",
	"switch_display", "menu", "save", "load",
};

const char* const kDefaultKeys[kBindableCommandCount] = {
	"W", "S", "A", "D", "Q", "Escape", "Return",
	"1", "2", "3", "4",
	"5", "6", "7", "8", "9", "0",
	"Tab", "M", "F5", "F9",
};

// модификаторы в комбинациях вида "Shift+S"; в этом порядке они идут в нормализованной записи
const int kKeyModifierCount = 4;
const char* const kKeyModifierNames[kKeyModifierCount] = { "CTRL", "SHIFT", "ALT", "CMD" };

Key_bindings::Key_bindings() {
	for (int i = 0; i < kBindableCommandCount; i++) {
		bind((Command) i, kDefaultKeys[i]);
	}
}

Key_bindings::Key_bindings(const string& path) {
	ifstream in(path);
	if (!in) {
		throw runtime_error("cannot open " + path);
	}
	string line;
	int line_number = 0;
	while (getline(in, line)) {
		line_number++;
		line = trim(line);
		if (!line.empty() && line[0] != '#') {
			parse_line(line, line_number);
		}
	}
	for (int i = 0; i < kBindableCommandCount; i++) {
		if (!keys_.has((Command) i)) {
			throw runtime_error("no key for command '" + string(kCommandNames[i]) + "'");
		}
	}
}

void Key_bindings::parse_line(const string& line, int line_number) {
	// первое слово — команда, остаток строки — название клавиши
	size_t space = line.find_first_of(" \t");
	if (space == string::npos) {
		throw runtime_error("line " + to_string(line_number) + ": expected '<command> <key>'");
	}
	string name = line.substr(0, space);
	string key = trim(line.substr(space));

	int index = 0;
	while (index < kBindableCommandCount && name != kCommandNames[index]) {
		index++;
	}
	if (index == kBindableCommandCount) {
		throw runtime_error("line " + to_string(line_number) + ": unknown command '" + name + "'");
	}
	bind((Command) index, key);
}

string Key_bindings::to_upper(string text) const {
	for (char& symbol : text) {
		symbol = (char) toupper((unsigned char) symbol);
	}
	return text;
}

string Key_bindings::trim(const string& text) const {
	size_t first = text.find_first_not_of(" \t\r");
	if (first == string::npos) {
		return "";
	}
	size_t last = text.find_last_not_of(" \t\r");
	return text.substr(first, last - first + 1);
}

bool Key_bindings::cut_modifier(string& combo, int& modifier) const {
	string upper = to_upper(combo);
	for (int i = 0; i < kKeyModifierCount; i++) {
		size_t length = strlen(kKeyModifierNames[i]);
		if (upper.compare(0, length, kKeyModifierNames[i]) != 0) {
			continue;
		}
		// после модификатора должен идти '+' и ещё хотя бы одна клавиша
		size_t plus = upper.find_first_not_of(" \t", length);
		if (plus == string::npos || upper[plus] != '+' || trim(upper.substr(plus + 1)).empty()) {
			continue;
		}
		combo = trim(combo.substr(plus + 1));
		modifier = i;
		return true;
	}
	return false;
}

string Key_bindings::get_key_name(const string& combo) const {
	string key = trim(combo);
	int modifier = 0;
	while (cut_modifier(key, modifier)) {
	}
	return key;
}

string Key_bindings::normalize(const string& combo) const {
	bool used[kKeyModifierCount] = {};
	string key = trim(combo);
	int modifier = 0;
	while (cut_modifier(key, modifier)) {
		used[modifier] = true;
	}
	string result;
	for (int i = 0; i < kKeyModifierCount; i++) {
		if (used[i]) {
			result += string(kKeyModifierNames[i]) + "+";
		}
	}
	return result + to_upper(key);
}

string Key_bindings::get_command_name(Command command) const {
	int index = (int) command;
	return index >= 0 && index < kBindableCommandCount ? kCommandNames[index] : "quit";
}

void Key_bindings::bind(Command command, const string& key) {
	string name = get_command_name(command);
	if (keys_.has(command)) {
		throw runtime_error("command '" + name + "' has several keys: " + keys_.get(command) + ", " + key);
	}
	string normalized = normalize(key);
	if (commands_.count(normalized) > 0) {
		throw runtime_error("key '" + key + "' is bound to several commands: "
							+ get_command_name(commands_[normalized]) + ", " + name);
	}
	commands_[normalized] = command;
	keys_.set(command, key);
}

bool Key_bindings::find_command(const string& key, Command& command) const {
	auto found = commands_.find(normalize(key));
	if (found == commands_.end()) {
		return false;
	}
	command = found->second;
	return true;
}

const Key_labels& Key_bindings::get_labels() const {
	return keys_;
}
