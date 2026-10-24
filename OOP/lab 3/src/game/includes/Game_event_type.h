#ifndef OOP_PROJECT_GAME_EVENT_TYPE_H
#define OOP_PROJECT_GAME_EVENT_TYPE_H

// Вид игрового события
enum class Game_event_type {
	Info, // что происходит в игре: ходы, способности, ранги, уровни
	Error, // недопустимое действие игрока: стена, граница поля, неверная цель
};

#endif
