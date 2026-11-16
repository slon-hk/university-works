#ifndef OOP_PROJECT_PLAYER_STATE_H
#define OOP_PROJECT_PLAYER_STATE_H

#include <cstddef>
#include <memory>
#include <vector>

#include "../../abilities/includes/Robot_abilities.h"
#include "../../items/includes/Inventory.h"
#include "../../robot/includes/Robot.h"

class Player_state {
public:
	static constexpr std::size_t kInventoryCapacity = 6;

private:
	Robot robot_;
	std::vector<std::shared_ptr<Robot_abilities>> abilities_;
	Inventory inventory_;

public:
	Player_state();
	Player_state(const Robot& robot, const std::vector<std::shared_ptr<Robot_abilities>>& abilities,
				 const Inventory& inventory);

	const Robot& get_robot() const;
	const std::vector<std::shared_ptr<Robot_abilities>>& get_abilities() const;
	const Inventory& get_inventory() const;

	// статусы снимаются (если не keep_statuses)
	void prepare_for_next_level(bool keep_statuses = false);
};

#endif
