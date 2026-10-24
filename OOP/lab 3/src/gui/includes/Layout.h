#ifndef OOP_PROJECT_LAYOUT_H
#define OOP_PROJECT_LAYOUT_H

#include "SDL.h"

// Геометрия окна: поле слева, боковая панель справа, панель способностей
// снизу. Переводит координаты клеток в пиксели.
class Layout {
private:
	int board_columns_;
	int board_rows_;

public:
	Layout(int board_columns, int board_rows);

	float get_cell_size() const;
	float get_board_width() const;
	float get_board_height() const;
	int get_window_width() const;
	int get_window_height() const;

	// прямоугольник клетки (x, y) в пикселях
	SDL_FRect get_cell_rect(int x, int y) const;

	SDL_FRect get_side_panel() const;
	SDL_FRect get_ability_bar() const;
	SDL_FRect get_ability_slot(int slot) const;
	SDL_FRect get_inventory_slot(int slot) const;
	// левый отступ текста в боковой панели
	float get_text_x() const;
	// x-координата подсказок справа от слотов способностей
	float get_hint_x() const;
};

#endif
