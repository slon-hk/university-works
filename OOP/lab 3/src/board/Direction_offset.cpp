#include "includes/Direction_offset.h"

Direction_offset::Direction_offset(Direction direction) : dx_(0), dy_(0) {
	if (direction == Direction::UP) {
		dy_ = -1;
	} else if (direction == Direction::DOWN) {
		dy_ = 1;
	} else if (direction == Direction::LEFT) {
		dx_ = -1;
	} else {
		dx_ = 1;
	}
}

int Direction_offset::get_dx() const {
	return dx_;
}

int Direction_offset::get_dy() const {
	return dy_;
}
