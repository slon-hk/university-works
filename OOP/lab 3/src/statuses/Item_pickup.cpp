#include "includes/Item_pickup.h"

#include <stdexcept>

#include "../game/includes/Game_session.h"

using namespace std;

Item_pickup::Item_pickup(shared_ptr<Item> item) : item_(item), expired_(false) {
	if (item_ == nullptr) {
		throw runtime_error("Item_pickup requires an item");
	}
}

string Item_pickup::get_name() const {
	return "Item";
}

string Item_pickup::get_description() const {
	return item_->get_name();
}

shared_ptr<Robot_status> Item_pickup::clone() const {
	return make_shared<Item_pickup>(*this);
}

void Item_pickup::on_apply(Game_session& session, int robot_index) {
	// подбирать предметы может только игрок; для остальных предмет просто пропадает
	if (session.get_robots().is_player(robot_index)) {
		session.add_item(item_);
	}
	expired_ = true;
}

bool Item_pickup::is_expired() const {
	return expired_;
}
