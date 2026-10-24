#include "includes/Robot_serializer.h"

#include <stdexcept>
#include <string>

using namespace std;

const int kRobotValueCount = 12;
const long long kMaxRobotStatuses = 10;

void Robot_serializer::write(Save_writer& out, const Robot& robot) const {
	out.write_int(robot.get_current_health());
	out.write_int(robot.get_max_health());
	// бонусы к атаке дадут сами статусы после загрузки
	out.write_int(robot.get_attack_power() - robot.get_attack_bonus());
	out.write_int(robot.get_current_energy());
	out.write_int(robot.get_max_energy());
	out.write_int(robot.get_current_experience());
	out.write_int(robot.get_experience_to_next_rank());
	out.write_int(robot.get_rank());
	out.write_int(robot.get_vision_radius());
	out.write_int(robot.get_heal_power());
	out.write_int(robot.get_speed());
	out.write_int(robot.get_movement_points());
	out.write_int((int) robot.get_relation());
	out.end_line();
	out.write_key("statuses");
	out.write_int(robot.get_status_count());
	out.end_line();
	for (int i = 0; i < robot.get_status_count(); i++) {
		statuses_.write(out, robot.get_status(i));
	}
}

Robot Robot_serializer::read(Save_reader& in) const {
	long long values[kRobotValueCount];
	const char* names[kRobotValueCount] = { "health", "max health", "attack", "energy", "max energy",
											"experience", "experience to next rank", "rank", "vision",
											"heal power", "speed", "movement points" };
	for (int i = 0; i < kRobotValueCount; i++) {
		values[i] = in.read_int(0, Robot::kMaxStat, string("robot ") + names[i]);
	}
	long long relation = in.read_int(-1, 1, "robot relation");

	if (values[0] > values[1] || values[1] == 0) {
		throw runtime_error("robot health exceeds max health");
	}
	if (values[3] > values[4]) {
		throw runtime_error("robot energy exceeds max energy");
	}
	if (values[6] == 0 || values[5] >= values[6]) {
		throw runtime_error("robot experience must be below experience to next rank");
	}
	if (values[7] == 0) {
		throw runtime_error("robot rank must be positive");
	}
	if (values[11] > values[10]) {
		throw runtime_error("robot movement points exceed speed");
	}
	Robot robot((size_t) values[0], (size_t) values[1], (size_t) values[2], (size_t) values[3],
				(size_t) values[4], (size_t) values[5], (size_t) values[6], (size_t) values[7],
				(size_t) values[8], (size_t) values[9], (size_t) values[10], (Relation) relation);
	robot.restore_movement_points();
	robot.spend_movement_points((int) (values[10] - values[11]));

	in.expect("statuses");
	long long count = in.read_int(0, kMaxRobotStatuses, "robot status count");
	for (long long i = 0; i < count; i++) {
		robot.add_status(statuses_.read(in, true));
	}
	return robot;
}
