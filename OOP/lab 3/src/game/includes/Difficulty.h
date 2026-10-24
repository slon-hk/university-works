#ifndef OOP_PROJECT_DIFFICULTY_H
#define OOP_PROJECT_DIFFICULTY_H

#include <string>
#include <vector>

#include "Difficulty_level.h"
#include "Modifier.h"

// Сложность игры: сила вражеских роботов и набор модификаторов.
class Difficulty {
private:
	Difficulty_level level_;
	std::vector<Modifier> modifiers_;

public:
	// Normal без модификаторов
	Difficulty();
	// случайные модификаторы: Easy — 0, Normal — 1, Hard — 2
	explicit Difficulty(Difficulty_level level);
	// заданные модификаторы без повторов (повторы отбрасываются)
	Difficulty(Difficulty_level level, const std::vector<Modifier>& modifiers);

	Difficulty_level get_level() const;
	std::string get_name() const;
	// процент от базовых HP и атаки врагов и заводов: 75 / 100 / 150
	int get_enemy_power_percent() const;

	bool has_modifier(Modifier modifier) const;
	const std::vector<Modifier>& get_modifiers() const;
	// описание любого модификатора (не обязательно включённого)
	std::string get_modifier_description(Modifier modifier) const;
};

#endif
