#ifndef OOP_PROJECT_ROBOT_UNIT_H
#define OOP_PROJECT_ROBOT_UNIT_H

#include <memory>
#include <vector>

#include "../../abilities/includes/Robot_abilities.h"
#include "../../robot/includes/Robot.h"

// Робот на поле вместе с координатами и способностями.
// Инвариант: в списке способностей нет пустых указателей.
class Robot_unit {
private:
	Robot robot_;
	int x_;
	int y_;
	std::vector<std::shared_ptr<Robot_abilities>> abilities_;

public:
	// пустые способности отбрасываются
	Robot_unit(const Robot& robot, int x, int y, const std::vector<std::shared_ptr<Robot_abilities>>& abilities);

	Robot& get_robot();
	const Robot& get_robot() const;
	int get_x() const;
	int get_y() const;
	void move_to(int x, int y);
	// погибший робот клетку не занимает
	bool occupies(int x, int y) const;

	const std::vector<std::shared_ptr<Robot_abilities>>& get_abilities() const;
	// пустые способности отбрасываются
	void set_abilities(const std::vector<std::shared_ptr<Robot_abilities>>& abilities);
	void add_ability(std::shared_ptr<Robot_abilities> ability);
};

#endif
