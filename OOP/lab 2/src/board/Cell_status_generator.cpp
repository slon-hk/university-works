#include "includes/Cell_status_generator.h"

#include <cstdlib>
#include <utility>
#include <vector>

#include "../items/includes/Attack_potion.h"
#include "../statuses/includes/Burn.h"
#include "../items/includes/Energy_potion.h"
#include "../items/includes/Health_potion.h"
#include "../statuses/includes/Item_pickup.h"
#include "../statuses/includes/Overload.h"
#include "../statuses/includes/Shield.h"
#include "../statuses/includes/Slow.h"

using namespace std;

// шансы в процентах: 8% — один статус, 2% — два статуса
const int kOneStatusChance = 8;
const int kTwoStatusChance = 2;
const int kBurnTurns = 3;
const int kShieldAmount = 5;

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
		if (roll < kTwoStatusChance) {
			count = 2;
		} else if (roll < kTwoStatusChance + kOneStatusChance) {
			count = 1;
		}
		for (int k = 0; k < count; k++) {
			board.add_square_status(x, y, create_random_status());
		}
	}
}

shared_ptr<Robot_status> Cell_status_generator::create_random_status() const {
	// равновероятно один из пяти вариантов; последний — предмет
	int type = rand() % 5;
	if (type == 0) {
		return make_shared<Slow>();
	}
	if (type == 1) {
		return make_shared<Overload>();
	}
	if (type == 2) {
		return make_shared<Burn>(kBurnTurns);
	}
	if (type == 3) {
		return make_shared<Shield>(kShieldAmount);
	}
	return make_shared<Item_pickup>(create_random_item());
}

shared_ptr<Item> Cell_status_generator::create_random_item() const {
	int type = rand() % 3;
	if (type == 0) {
		return make_shared<Health_potion>();
	}
	if (type == 1) {
		return make_shared<Energy_potion>();
	}
	return make_shared<Attack_potion>();
}
