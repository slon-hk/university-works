#ifndef OOP_PROJECT_ROBOT_ABILITIES_H
#define OOP_PROJECT_ROBOT_ABILITIES_H

#include <cstddef>
#include <string>

class Game_session;

// Интерфейс способности робота. Способность тратит энергию, может требовать
// выбора целевой клетки и прокачивается при повышении ранга.
// user_index — индекс использующего робота в Game_session.
class Robot_abilities {
public:
	virtual ~Robot_abilities() = default;

	virtual std::string get_name() const = 0;
	// стоимость применения в энергии
	virtual std::size_t get_energy_cost() const = 0;
	// радиус действия (для подсветки на поле)
	virtual int get_radius() const = 0;
	// true — игрок должен кликнуть по целевой клетке
	virtual bool needs_target() const = 0;

	// проверка, применима ли способность (живой ли робот, есть ли цель и т.д.);
	// энергию проверяет Game_session::use_ability
	virtual bool can_use(const Game_session& session, int user_index,
						 int target_x, int target_y) const = 0;
	// непосредственно эффект способности
	virtual void use(Game_session& session, int user_index,
					 int target_x, int target_y) const = 0;

	// Прокачка: текущий уровень, описание следующего улучшения и само улучшение.
	virtual int get_level() const = 0;
	virtual std::string get_upgrade_description() const = 0;
	virtual void upgrade() = 0;
};

#endif //OOP_PROJECT_ROBOT_ABILITIES_H
