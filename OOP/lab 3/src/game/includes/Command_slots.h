#ifndef OOP_PROJECT_COMMAND_SLOTS_H
#define OOP_PROJECT_COMMAND_SLOTS_H

#include "Command.h"

// Перевод команд-слотов в номера и обратно: Ability_1..Ability_4 — слоты
// способностей 0..3 (в меню — пункты), Item_1..Item_6 — слоты инвентаря 0..5.
class Command_slots {
public:
	int get_ability_slot_count() const;
	int get_item_slot_count() const;

	bool is_ability(Command command) const;
	bool is_item(Command command) const;
	// номер слота команды; бросает invalid_argument, если команда не из группы
	int get_ability_slot(Command command) const;
	int get_item_slot(Command command) const;
	// команда слота; бросает out_of_range при неверном номере
	Command get_ability_command(int slot) const;
	Command get_item_command(int slot) const;
};

#endif
