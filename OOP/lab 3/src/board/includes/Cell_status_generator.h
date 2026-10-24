#ifndef OOP_PROJECT_CELL_STATUS_GENERATOR_H
#define OOP_PROJECT_CELL_STATUS_GENERATOR_H

#include <memory>

#include "Cell_status_type.h"
#include "Game_board.h"
#include "../../items/includes/Item_catalog.h"
#include "../../statuses/includes/Robot_status.h"

// Случайно раскладывает статусы и предметы по свободным клеткам поля
// при построении уровня.
class Cell_status_generator {
private:
	int one_status_chance_; // шанс (в процентах), что на клетке будет один статус
	int two_status_chance_; // шанс, что на клетке будет два статуса

	Item_catalog items_;

	std::shared_ptr<Robot_status> create_random_status() const;

public:
	// шансы зажимаются в [0, 100], их сумма — не больше 100
	explicit Cell_status_generator(int one_status_chance = 8, int two_status_chance = 2);

	// проходит по всем клеткам (кроме препятствий) и с заданной
	// вероятностью кладёт на них 1–2 случайных статуса
	void generate(Game_board& board) const;
	// статус указанного вида (для Item — случайный предмет)
	std::shared_ptr<Robot_status> create_status(Cell_status_type type) const;
};

#endif
