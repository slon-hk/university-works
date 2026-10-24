#include "includes/Key_labels.h"

#include <stdexcept>
#include <string>

using namespace std;

void Key_labels::set(Command command, const string& key) {
	labels_[command] = key;
}

bool Key_labels::has(Command command) const {
	return labels_.count(command) > 0;
}

const string& Key_labels::get(Command command) const {
	auto found = labels_.find(command);
	if (found == labels_.end()) {
		throw out_of_range("no key for command " + to_string((int) command));
	}
	return found->second;
}

const string& Key_labels::get_ability(int slot) const {
	return get(slots_.get_ability_command(slot));
}

const string& Key_labels::get_item(int slot) const {
	return get(slots_.get_item_command(slot));
}

const map<Command, string>& Key_labels::get_all() const {
	return labels_;
}
