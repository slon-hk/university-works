#ifndef OOP_PROJECT_ITEM_PICKUP_H
#define OOP_PROJECT_ITEM_PICKUP_H

#include <memory>

#include "../../items/includes/Item.h"
#include "Base_status.h"

// «Статус»-обёртка над предметом, лежащим на клетке. При наложении на игрока
// кладёт предмет в инвентарь и сразу истекает, поэтому на роботе не остаётся.
class Item_pickup : public Base_status {
private:
	std::shared_ptr<Item> item_;
	bool expired_;

public:
	explicit Item_pickup(std::shared_ptr<Item> item);

	std::string get_name() const override;
	std::string get_description() const override;
	std::shared_ptr<Robot_status> clone() const override;
	void on_apply(Game_session& session, int robot_index) override;

	bool is_expired() const override;
};

#endif
