#ifndef OOP_PROJECT_GAME_PHASE_H
#define OOP_PROJECT_GAME_PHASE_H

// Этап игры, от которого зависит, как обрабатываются команды и что рисуется
enum class Game_phase {
	Start_ability, // выбор стартовой способности
	Playing, // идёт уровень (в том числе выбор награды за ранг)
	Level_complete, // уровень пройден, ждём подтверждения
	Game_over, // робот игрока погиб, ждём подтверждения
	Victory, // пройдены все уровни, ждём подтверждения
	Finished, // игра окончена или игрок вышел в меню
};

#endif
