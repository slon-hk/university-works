#include "includes/Display_mode_selector.h"

using namespace std;

Display_mode_selector::Display_mode_selector() : mode_(Display_mode::Standard) {
}

Display_mode Display_mode_selector::get_mode() const {
	return mode_;
}

string Display_mode_selector::get_name() const {
	if (mode_ == Display_mode::Enemies) {
		return "Enemies";
	}
	if (mode_ == Display_mode::Obstacles) {
		return "Obstacles";
	}
	if (mode_ == Display_mode::Status_cells) {
		return "Status cells";
	}
	return "Standard";
}

void Display_mode_selector::next() {
	if (mode_ == Display_mode::Standard) {
		mode_ = Display_mode::Enemies;
	} else if (mode_ == Display_mode::Enemies) {
		mode_ = Display_mode::Obstacles;
	} else if (mode_ == Display_mode::Obstacles) {
		mode_ = Display_mode::Status_cells;
	} else {
		mode_ = Display_mode::Standard;
	}
}
