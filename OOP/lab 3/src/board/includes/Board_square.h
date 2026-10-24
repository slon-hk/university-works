#ifndef OOP_PROJECT_BOARD_SQUARE_H
#define OOP_PROJECT_BOARD_SQUARE_H

#include <cstddef>
#include <memory>
#include <vector>

#include "../../statuses/includes/Robot_status.h"

// Одна клетка поля. Инвариант: стоимость прохода не меньше 1,
// в списке статусов нет пустых указателей.
class Board_square {
private:
	int speed_level_; // сколько очков движения стоит зайти в клетку
	bool border_; // непроходимое препятствие
	bool visible_; // клетка сейчас в поле зрения игрока
	bool discovered_; // препятствие уже было увидено (остаётся на карте в тумане)
	std::vector<std::shared_ptr<Robot_status>> statuses_; // эффекты/предметы, которые получит вошедший робот

public:
	Board_square();

	int get_speed_level() const;
	// значения меньше 1 зажимаются до 1
	void set_speed_level(int level);
	bool is_border() const;
	void set_border(bool border);
	bool is_visible() const;
	void set_visible(bool visible);
	bool is_discovered() const;
	void set_discovered(bool discovered);

	const std::vector<std::shared_ptr<Robot_status>>& get_statuses() const;
	// пустой статус не добавляется
	void add_status(std::shared_ptr<Robot_status> status);
	// бросает out_of_range при неверном номере
	void remove_status(std::size_t index);
	void clear_statuses();
};

#endif
