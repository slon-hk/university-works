#ifndef OOP_PROJECT_MAP_EDITOR_H
#define OOP_PROJECT_MAP_EDITOR_H

#include <memory>
#include <vector>

#include "../../board/includes/Cell_status_generator.h"
#include "../../board/includes/Game_board.h"
#include "Game_session.h"
#include "Placed_factory.h"
#include "Robot_unit.h"
#include "../../robot/includes/Robot.h"
#include "../../robot/includes/Robot_factory.h"

// Конструктор уровня: накапливает описание карты (препятствия, стоимость
// клеток, роботы, заводы), а build() собирает из него готовую Game_session.
class Map_editor {
private:
	Game_board board_;
	std::vector<Robot_unit> player_; // 0 или 1 элемент — игрок
	std::vector<Robot_unit> robots_;
	std::vector<Placed_factory> factories_;
	Cell_status_generator status_generator_;

public:
	Map_editor(int width, int height);

	void add_obstacle(int x, int y);
	void add_speed_level(int x, int y, int level);
	// «вязкая» клетка со стандартной стоимостью прохода
	void add_slow_cell(int x, int y);
	// кладёт статус на клетку (вдобавок к случайно сгенерированным)
	void add_cell_status(int x, int y, std::shared_ptr<Robot_status> status);
	// шансы появления случайных статусов на клетках при build()
	void set_status_chances(int one_status_chance, int two_status_chance);
	// задаёт игрока (повторный вызов заменяет предыдущего)
	void set_player(Robot robot, int x, int y);
	void add_robot(Robot robot, int x, int y);
	void add_factory(Robot_factory factory, int x, int y);

	// генерирует статусы на клетках и расставляет объекты; объекты на занятых
	// или непроходимых клетках молча пропускаются
	Game_session build() const;
};

#endif
