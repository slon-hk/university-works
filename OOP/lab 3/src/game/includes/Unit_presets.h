#ifndef OOP_PROJECT_UNIT_PRESETS_H
#define OOP_PROJECT_UNIT_PRESETS_H

#include <cstddef>

#include "../../robot/includes/Robot.h"
#include "../../robot/includes/Robot_factory.h"

// Готовые шаблоны роботов и заводов для генераторов уровней. Чем выше
// ранг/уровень, тем сильнее объект; power_percent (зависит от сложности)
// задаёт процент HP и атаки врагов и заводов от базовых.
class Unit_presets {
private:
	int power_percent_;

	// value * power_percent_ / 100, но не меньше 1
	std::size_t scale(std::size_t value) const;

public:
	// power_percent зажимается снизу до 1
	explicit Unit_presets(int power_percent = 100);

	// враг ранга rank (>= 1): HP, атака, энергия, обзор и скорость растут с рангом
	Robot create_enemy(int rank) const;
	Robot create_ally() const;
	// завод уровня level (>= 1): чаще и больше производит, прочнее
	Robot_factory create_factory(int level) const;
};

#endif
