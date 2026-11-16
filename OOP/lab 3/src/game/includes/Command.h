#ifndef OOP_PROJECT_COMMAND_H
#define OOP_PROJECT_COMMAND_H

enum class Command {
	Move_up,
	Move_down,
	Move_left,
	Move_right,
	Use_ability,
	Cancel,
	End_turn,
	Ability_1,
	Ability_2,
	Ability_3,
	Ability_4,
	Item_1,
	Item_2,
	Item_3,
	Item_4,
	Item_5,
	Item_6,
	Switch_display,
	Menu,
	Save,
	Load,
	Quit,
};

// количество команд, которым назначаются клавиши (все, кроме Quit)
const int kBindableCommandCount = (int) Command::Quit;

#endif
