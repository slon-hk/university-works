#ifndef OOP_PROJECT_RANDOM_LEVEL_GENERATOR_H
#define OOP_PROJECT_RANDOM_LEVEL_GENERATOR_H

#include <string>
#include <vector>

#include "Difficulty.h"
#include "Game_session.h"
#include "Map_editor.h"

// Случайные уровни. Сложность растёт с номером уровня: враги сразу
// высокого ранга и их больше, заводы сильнее, клеток со статусами больше.
// Сложность игры дополнительно меняет силу врагов и заводов.
class Random_level_generator {
private:
	// занятые клетки: препятствия, роботы, заводы
	using Occupancy = std::vector<std::vector<bool>>;

	// случайная незанятая клетка, у которой квадрат size x size свободен
	// и которая не ближе min_distance к (from_x, from_y); false, если не нашлась
	bool find_free_cell(const Occupancy& used, int size, int from_x, int from_y, int min_distance,
						int& out_x, int& out_y) const;
	void mark(Occupancy& used, int x, int y, int size) const;
	// короткие стены из препятствий
	void add_walls(Map_editor& editor, Occupancy& used, int count) const;
	// прямоугольные участки вязких клеток
	void add_slow_areas(Map_editor& editor, int count) const;

public:
	std::string get_name() const;
	Game_session create(int level_number, const Difficulty& difficulty) const;
};

#endif
