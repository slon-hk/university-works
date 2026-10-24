#ifndef OOP_PROJECT_PLAYER_STATE_H
#define OOP_PROJECT_PLAYER_STATE_H

#include <cstddef>
#include <memory>
#include <vector>

#include "../../abilities/includes/Robot_abilities.h"
#include "../../items/includes/Inventory.h"
#include "../../robot/includes/Robot.h"

// Всё, что игрок переносит с уровня на уровень: робот (ранг, опыт, здоровье),
// способности вместе с улучшениями и инвентарь.
// Инвариант: робот нейтральный, способности не пустые указатели и не
// повторяются, вместимость инвентаря равна kInventoryCapacity.
class Player_state {
public:
	static constexpr std::size_t kInventoryCapacity = 6;

private:
	Robot robot_;
	std::vector<std::shared_ptr<Robot_abilities>> abilities_;
	Inventory inventory_;

public:
	// состояние для первого уровня: стартовый робот без способностей
	// и пустой инвентарь
	Player_state();
	// бросает invalid_argument при нарушении инварианта
	Player_state(const Robot& robot, const std::vector<std::shared_ptr<Robot_abilities>>& abilities,
				 const Inventory& inventory);

	const Robot& get_robot() const;
	const std::vector<std::shared_ptr<Robot_abilities>>& get_abilities() const;
	const Inventory& get_inventory() const;

	// правила перехода на следующий уровень: статусы снимаются (если не
	// keep_statuses), энергия и очки движения восстанавливаются
	void prepare_for_next_level(bool keep_statuses = false);
};

#endif
