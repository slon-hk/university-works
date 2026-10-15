#include "includes/Demo_level.h"

#include "includes/Map_editor.h"
#include "../robot/includes/Robot.h"
#include "../robot/includes/Robot_factory.h"

const int kBoardSize = 32;

Game_session Demo_level::create() const {
	Map_editor editor(kBoardSize, kBoardSize);

	// параметры: HP, макс. HP, атака, энергия, макс. энергия, опыт, опыт до ранга,
	// ранг, обзор, сила лечения, скорость, отношение
	Robot player(100, 100, 10, 10, 10, 0, 50, 1, 7, 10, 5, Relation::Neutral);
	editor.set_player(player, 10, 10);

	Robot enemy(100, 100, 10, 10, 10, 0, 100, 1, 5, 10, 2, Relation::Enemy);
	editor.add_robot(enemy, 13, 10);
	editor.add_robot(enemy, 6, 16);

	Robot ally(100, 100, 10, 10, 10, 0, 100, 1, 5, 10, 2, Relation::Ally);
	editor.add_robot(ally, 11, 20);

	// стены из препятствий
	for (int y = 13; y <= 17; y++) {
		editor.add_obstacle(15, y);
	}
	for (int x = 4; x <= 7; x++) {
		editor.add_obstacle(x, 6);
	}

	// участок, где каждый шаг стоит 3 очка движения
	for (int x = 18; x <= 21; x++) {
		editor.add_speed_level(x, 8, 3);
		editor.add_speed_level(x, 9, 3);
	}

	Robot factory_unit(60, 60, 8, 10, 10, 0, 100, 1, 5, 10, 2, Relation::Enemy);
	// 1 робот за раз, уровень 1, каждые 5 ходов
	Robot_factory factory(factory_unit, 1, 1, 5);
	editor.add_factory(factory, 22, 15);

	return editor.build();
}
