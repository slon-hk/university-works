#ifndef OOP_PROJECT_COMMAND_H
#define OOP_PROJECT_COMMAND_H

// Игровые команды, в которые система ввода превращает действия пользователя.
// Смысл команды зависит от экрана: в меню Ability_1..Ability_4 выбирают
// пункт, а Cancel — выход; на экранах-сообщениях любая команда — «далее».
enum class Command {
	Move_up,
	Move_down,
	Move_left,
	Move_right,
	Use_ability, // применить выбранную способность / подтвердить цель
	Cancel, // отменить выбор цели
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
	Switch_display, // следующий режим подсветки поля
	Menu, // выйти в главное меню
	Save, // сохранить игру в файл
	Load, // загрузить игру из файла
	Quit, // закрытие окна; на клавишу не назначается
};

// количество команд, которым назначаются клавиши (все, кроме Quit)
const int kBindableCommandCount = (int) Command::Quit;

#endif
