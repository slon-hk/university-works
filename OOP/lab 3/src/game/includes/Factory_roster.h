#ifndef OOP_PROJECT_FACTORY_ROSTER_H
#define OOP_PROJECT_FACTORY_ROSTER_H

#include <vector>

#include "../../robot/includes/Robot_factory.h"
#include "Placed_factory.h"

// Все заводы уровня. Разрушенный завод удаляется, так что любой
// оставшийся — действующий. Неверный индекс — out_of_range.
class Factory_roster {
private:
	std::vector<Placed_factory> factories_;

public:
	// клетки проверяет вызывающий (Game_session)
	void add(const Robot_factory& factory, int x, int y);
	void remove(int index);

	int get_count() const;
	Robot_factory& get_factory(int index);
	const Robot_factory& get_factory(int index) const;
	int get_x(int index) const;
	int get_y(int index) const;
	// индекс завода, занимающего клетку, или -1
	int find_at(int x, int y) const;
	bool has_enemy_factories() const;
};

#endif
