#ifndef OOP_PROJECT_COMMAND_SLOTS_H
#define OOP_PROJECT_COMMAND_SLOTS_H

#include "Command.h"

class Command_slots {
public:
	int get_ability_slot_count() const;
	int get_item_slot_count() const;

	bool is_ability(Command command) const;
	bool is_item(Command command) const;

	int get_ability_slot(Command command) const;
	int get_item_slot(Command command) const;

	Command get_ability_command(int slot) const;
	Command get_item_command(int slot) const;
};

#endif
