#ifndef OOP_PROJECT_MAP_EDITOR_H
#define OOP_PROJECT_MAP_EDITOR_H

#include <vector>

#include "../../board/includes/Cell_status_generator.h"
#include "../../board/includes/Game_board.h"
#include "Game_session.h"
#include "../../robot/includes/Robot.h"
#include "../../robot/includes/Robot_factory.h"

// Конструктор уровня: накапливает описание карты (препятствия, стоимость
// клеток, роботы, заводы), а build() собирает из него готовую Game_session.
class Map_editor {
private:
	struct Robot_placement {
		Robot robot;
		int x;
		int y;
	};

	struct Factory_placement {
		Robot_factory factory;
		int x;
		int y;
	};

	Game_board board_;
	std::vector<Robot_placement> player_; // 0 или 1 элемент — игрок
	std::vector<Robot_placement> robots_;
	std::vector<Factory_placement> factories_;
	Cell_status_generator status_generator_;

public:
	Map_editor(int width, int height);

	void add_obstacle(int x, int y);
	void add_speed_level(int x, int y, int level);
	// задаёт игрока (повторный вызов заменяет предыдущего)
	void set_player(Robot robot, int x, int y);
	void add_robot(Robot robot, int x, int y);
	void add_factory(Robot_factory factory, int x, int y);

	// генерирует статусы на клетках и расставляет объекты; объекты на занятых
	// или непроходимых клетках молча пропускаются
	Game_session build() const;
};

#endif
