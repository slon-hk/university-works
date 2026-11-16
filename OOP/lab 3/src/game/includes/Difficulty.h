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

	Difficulty(Difficulty_level level);

	Difficulty(Difficulty_level level, const std::vector<Modifier>& modifiers);

	Difficulty_level get_level() const;
	std::string get_name() const;

	int get_enemy_power_percent() const;

	bool has_modifier(Modifier modifier) const;
	const std::vector<Modifier>& get_modifiers() const;

	std::string get_modifier_description(Modifier modifier) const;
};

#endif
