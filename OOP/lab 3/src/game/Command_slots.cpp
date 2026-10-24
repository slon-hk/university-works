#include "includes/Command_slots.h"

#include <stdexcept>
#include <string>

using namespace std;

const int kAbilitySlotCount = (int) Command::Ability_4 - (int) Command::Ability_1 + 1;
const int kItemSlotCount = (int) Command::Item_6 - (int) Command::Item_1 + 1;

int Command_slots::get_ability_slot_count() const {
	return kAbilitySlotCount;
}

int Command_slots::get_item_slot_count() const {
	return kItemSlotCount;
}

bool Command_slots::is_ability(Command command) const {
	return command >= Command::Ability_1 && command <= Command::Ability_4;
}

bool Command_slots::is_item(Command command) const {
	return command >= Command::Item_1 && command <= Command::Item_6;
}

int Command_slots::get_ability_slot(Command command) const {
	if (!is_ability(command)) {
		throw invalid_argument("command is not an ability slot");
	}
	return (int) command - (int) Command::Ability_1;
}

int Command_slots::get_item_slot(Command command) const {
	if (!is_item(command)) {
		throw invalid_argument("command is not an item slot");
	}
	return (int) command - (int) Command::Item_1;
}

Command Command_slots::get_ability_command(int slot) const {
	if (slot < 0 || slot >= kAbilitySlotCount) {
		throw out_of_range("ability slot " + to_string(slot));
	}
	return (Command) ((int) Command::Ability_1 + slot);
}

Command Command_slots::get_item_command(int slot) const {
	if (slot < 0 || slot >= kItemSlotCount) {
		throw out_of_range("item slot " + to_string(slot));
	}
	return (Command) ((int) Command::Item_1 + slot);
}
