#ifndef OOP_PROJECT_CELL_STATUS_GENERATOR_H
#define OOP_PROJECT_CELL_STATUS_GENERATOR_H

#include <memory>

#include "Game_board.h"
#include "../../items/includes/Item.h"
#include "../../statuses/includes/Robot_status.h"

// Случайно раскладывает статусы и предметы по свободным клеткам поля
// при построении уровня.
class Cell_status_generator {
private:
	std::shared_ptr<Robot_status> create_random_status() const;
	std::shared_ptr<Item> create_random_item() const;

public:
	// проходит по всем клеткам (кроме препятствий) и с некоторой
	// вероятностью кладёт на них 1–2 случайных статуса
	void generate(Game_board& board) const;
};

#endif
