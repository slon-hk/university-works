#ifndef OOP_PROJECT_ABILITY_CATALOG_H
#define OOP_PROJECT_ABILITY_CATALOG_H

#include <memory>
#include <string>
#include <vector>

#include "Robot_abilities.h"

// Каталог всех способностей игрока. Индекс в каталоге совпадает с номером
// слота на панели способностей (клавиши 1–4).
class Ability_catalog {
public:
	static constexpr int kMaxLevel = 1000;

private:
	std::vector<std::string> names_;

public:
	Ability_catalog();

	int get_count() const;
	const std::string& get_name(int index) const;
	std::shared_ptr<Robot_abilities> create(int index) const;
	// способность по имени, улучшенная до level (1..kMaxLevel);
	// бросает runtime_error при неизвестном имени или неверном уровне
	std::shared_ptr<Robot_abilities> create(const std::string& name, int level) const;
	std::shared_ptr<Robot_abilities> create_enemy_ability() const;
};

#endif
