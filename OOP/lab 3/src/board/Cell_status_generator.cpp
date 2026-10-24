#include "includes/Cell_status_generator.h"

#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>

#include "../statuses/includes/Burn.h"
#include "../statuses/includes/Item_pickup.h"
#include "../statuses/includes/Overload.h"
#include "../statuses/includes/Shield.h"
#include "../statuses/includes/Slow.h"

using namespace std;

const int kShieldAmount = 5;

Cell_status_generator::Cell_status_generator(int one_status_chance, int two_status_chance) {
	two_status_chance_ = min(max(two_status_chance, 0), 100);
	one_status_chance_ = min(max(one_status_chance, 0), 100 - two_status_chance_);
}

void Cell_status_generator::generate(Game_board& board) const {
	vector<pair<int, int>> cells = board.get_all_coordinates();
	for (size_t i = 0; i < cells.size(); i++) {
		int x = cells[i].first;
		int y = cells[i].second;
		if (board.get_square_border(x, y)) {
			continue;
		}
		// 100 и решаем, сколько статусов положить на клетку
		int roll = rand() % 100;
		int count = 0;
		if (roll < two_status_chance_) {
			count = 2;
		} else if (roll < two_status_chance_ + one_status_chance_) {
			count = 1;
		}
		for (int k = 0; k < count; k++) {
			board.add_square_status(x, y, create_random_status());
		}
	}
}

shared_ptr<Robot_status> Cell_status_generator::create_random_status() const {
	// равновероятно один из пяти вариантов; последний — предмет
	return create_status((Cell_status_type) (rand() % kCellStatusTypeCount));
}

shared_ptr<Robot_status> Cell_status_generator::create_status(Cell_status_type type) const {
	if (type == Cell_status_type::Slow) {
		return make_shared<Slow>();
	}
	if (type == Cell_status_type::Overload) {
		return make_shared<Overload>();
	}
	if (type == Cell_status_type::Burn) {
		return make_shared<Burn>();
	}
	if (type == Cell_status_type::Shield) {
		return make_shared<Shield>(kShieldAmount);
	}
	return make_shared<Item_pickup>(items_.create_random());
}
