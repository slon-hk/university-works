#include "includes/Board_square.h"

#include <stdexcept>
#include <string>

using namespace std;

Board_square::Board_square() : speed_level_(1), border_(false), visible_(false), discovered_(false) {
}

int Board_square::get_speed_level() const {
	return speed_level_;
}

void Board_square::set_speed_level(int level) {
	speed_level_ = level < 1 ? 1 : level;
}

bool Board_square::is_border() const {
	return border_;
}

void Board_square::set_border(bool border) {
	border_ = border;
}

bool Board_square::is_visible() const {
	return visible_;
}

void Board_square::set_visible(bool visible) {
	visible_ = visible;
}

bool Board_square::is_discovered() const {
	return discovered_;
}

void Board_square::set_discovered(bool discovered) {
	discovered_ = discovered;
}

const vector<shared_ptr<Robot_status>>& Board_square::get_statuses() const {
	return statuses_;
}

void Board_square::add_status(shared_ptr<Robot_status> status) {
	if (status != nullptr) {
		statuses_.push_back(status);
	}
}

void Board_square::remove_status(size_t index) {
	if (index >= statuses_.size()) {
		throw out_of_range("square status " + to_string(index));
	}
	statuses_.erase(statuses_.begin() + index);
}

void Board_square::clear_statuses() {
	statuses_.clear();
}
