#ifndef OOP_PROJECT_MODIFIER_H
#define OOP_PROJECT_MODIFIER_H

// Модификаторы, усложняющие игру
enum class Modifier {
	Single_ability_per_turn, // игрок может применить только одну способность за ход
	Random_status_each_turn, // каждый ход на игрока накладывается случайный статус
	Statuses_persist, // статусы не снимаются при переходе на следующий уровень
	No_ability_upgrades, // за повышение ранга нельзя улучшать способности
};

const int kModifierCount = 4;

#endif
