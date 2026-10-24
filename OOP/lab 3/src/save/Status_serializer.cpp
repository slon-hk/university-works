#include "includes/Status_serializer.h"

#include <stdexcept>
#include <string>

#include "../robot/includes/Robot.h"
#include "../statuses/includes/Burn.h"
#include "../statuses/includes/Item_pickup.h"
#include "../statuses/includes/Overload.h"
#include "../statuses/includes/Shield.h"
#include "../statuses/includes/Slow.h"
#include "../statuses/includes/Strength.h"

using namespace std;

const long long kMaxStatusTurns = 1000;

void Status_serializer::write(Save_writer& out, const Robot_status& status) const {
	string name = status.get_name();
	out.write_string(name);
	if (name == "Item") {
		// у предмета на клетке описание — имя предмета
		out.write_string(status.get_description());
	} else if (name == "Burn") {
		const Burn* burn = dynamic_cast<const Burn*>(&status);
		out.write_int(status.get_amount());
		out.write_int(burn != nullptr ? burn->get_damage() : 0);
	} else if (name == "Strength") {
		out.write_int(status.get_amount());
		out.write_int(status.get_attack_bonus());
	} else {
		out.write_int(status.get_amount());
	}
	out.end_line();
}

shared_ptr<Robot_status> Status_serializer::read(Save_reader& in, bool on_robot) const {
	string name = in.read_string("status name");
	if (name == "Item" && !on_robot) {
		return make_shared<Item_pickup>(items_.create(in.read_string("item name")));
	}
	if (name == "Burn") {
		int turns = (int) in.read_int(1, kMaxStatusTurns, "Burn turns");
		return make_shared<Burn>(turns, (int) in.read_int(0, Robot::kMaxStat, "Burn damage"));
	}
	if (name == "Strength") {
		int turns = (int) in.read_int(1, kMaxStatusTurns, "Strength turns");
		return make_shared<Strength>(turns, (int) in.read_int(0, Robot::kMaxStat, "Strength bonus"));
	}
	if (name == "Shield") {
		return make_shared<Shield>((int) in.read_int(1, Robot::kMaxStat, "Shield amount"));
	}
	if (name == "Slow") {
		return make_shared<Slow>((int) in.read_int(1, kMaxStatusTurns, "Slow turns"));
	}
	if (name == "Overload") {
		in.read_int(1, 1, "Overload amount");
		return make_shared<Overload>();
	}
	throw runtime_error("unknown status '" + name + "'" + (on_robot ? " on robot" : " on cell"));
}
