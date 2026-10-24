#ifndef OOP_PROJECT_PLACED_FACTORY_H
#define OOP_PROJECT_PLACED_FACTORY_H

#include "../../robot/includes/Robot_factory.h"

// Завод на поле и координаты его левого верхнего угла
class Placed_factory {
private:
	Robot_factory factory_;
	int x_;
	int y_;

public:
	Placed_factory(const Robot_factory& factory, int x, int y);

	Robot_factory& get_factory();
	const Robot_factory& get_factory() const;
	int get_x() const;
	int get_y() const;
	// занимает ли завод клетку
	bool covers(int x, int y) const;
};

#endif
