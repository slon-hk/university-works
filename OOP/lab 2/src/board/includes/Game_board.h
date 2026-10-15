#ifndef OOP_PROJECT_GAME_BOARD_H
#define OOP_PROJECT_GAME_BOARD_H

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "../../statuses/includes/Robot_status.h"

// Игровое поле: прямоугольная сетка клеток. Каждая клетка хранит стоимость
// прохода, признак препятствия, видимость для игрока и лежащие на ней статусы.
// Координаты: x — столбец, y — строка; обращение за пределы поля безопасно.
class Game_board {
private:
	struct Square {
		int speed_level = 1; // сколько очков движения стоит зайти в клетку
		bool border = false; // непроходимое препятствие
		bool visible = false; // клетка сейчас в поле зрения игрока
		bool discovered = false; // препятствие уже было увидено (остаётся на карте в тумане)
		std::vector<std::shared_ptr<Robot_status>> statuses; // эффекты/предметы, которые получит вошедший робот
	};

	std::size_t width_;
	std::size_t height_;
	std::vector<std::vector<Square>> squares_;
	// пустой список, возвращаемый для клеток вне поля
	std::vector<std::shared_ptr<Robot_status>> no_statuses_;

public:
	// размеры зажимаются в диапазон [kMinSize, kMaxSize]
	Game_board(std::size_t width, std::size_t height);

	// возвращает {ширина, высота}
	std::pair<std::size_t, std::size_t> get_board_size() const;
	bool is_inside(int x, int y) const;
	// все пары (x, y) поля построчно
	std::vector<std::pair<int, int>> get_all_coordinates() const;

	// Сеттеры/геттеры свойств клетки. Для клеток вне поля сеттеры ничего
	// не делают, а геттеры возвращают значение по умолчанию (вне поля — препятствие).
	void set_square_speed_level(int x, int y, int level);
	int get_square_speed_level(int x, int y) const;

	void set_square_border(int x, int y, bool border);
	bool get_square_border(int x, int y) const;

	void set_square_visible(int x, int y, bool visible);
	bool get_square_visible(int x, int y) const;

	void set_square_discovered(int x, int y, bool discovered);
	bool get_square_discovered(int x, int y) const;

	void add_square_status(int x, int y, std::shared_ptr<Robot_status> status);
	const std::vector<std::shared_ptr<Robot_status>>& get_square_statuses(int x, int y) const;
	// удаляет один случайный статус с клетки (после того как робот на неё зашёл)
	void remove_random_square_status(int x, int y);
	void clear_square_statuses(int x, int y);
};

#endif
