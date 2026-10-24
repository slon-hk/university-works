#ifndef OOP_PROJECT_ROBOT_ROSTER_H
#define OOP_PROJECT_ROBOT_ROSTER_H

#include <memory>
#include <string>
#include <vector>

#include "../../abilities/includes/Robot_abilities.h"
#include "../../robot/includes/Robot.h"
#include "Robot_unit.h"

// Все роботы уровня с координатами и способностями. Игрок всегда под
// индексом kPlayerIndex (первый добавленный). Погибшие роботы остаются
// в списке, чтобы индексы не сдвигались, но клеток не занимают.
// Неверный индекс — out_of_range.
class Robot_roster {
public:
	static constexpr int kPlayerIndex = 0;

private:
	std::vector<Robot_unit> units_;

	Robot_unit& unit(int index);
	const Robot_unit& unit(int index) const;

public:
	// клетку проверяет вызывающий (Game_session)
	void add(const Robot& robot, int x, int y, const std::vector<std::shared_ptr<Robot_abilities>>& abilities);

	int get_count() const;
	bool is_empty() const;
	bool is_player(int index) const;
	// имя робота для журнала: "Player", "Enemy #N", "Ally #N"
	std::string get_name(int index) const;

	Robot& get_robot(int index);
	const Robot& get_robot(int index) const;
	int get_x(int index) const;
	int get_y(int index) const;
	void move_to(int index, int x, int y);
	// индекс живого робота в клетке или -1
	int find_at(int x, int y) const;
	// попадает ли клетка в квадрат радиуса radius вокруг робота index
	// (расстояние Чебышёва)
	bool is_in_radius(int index, int x, int y, int radius) const;
	bool has_living_enemies() const;

	int get_ability_count(int index) const;
	const Robot_abilities& get_ability(int robot_index, int ability_index) const;
	Robot_abilities& get_ability(int robot_index, int ability_index);
	std::shared_ptr<Robot_abilities> share_ability(int robot_index, int ability_index) const;
	// индекс способности по имени или -1
	int find_ability(int robot_index, const std::string& name) const;
	const std::vector<std::shared_ptr<Robot_abilities>>& get_abilities(int index) const;
	void set_abilities(int index, const std::vector<std::shared_ptr<Robot_abilities>>& abilities);
	void add_ability(int index, std::shared_ptr<Robot_abilities> ability);
};

#endif
