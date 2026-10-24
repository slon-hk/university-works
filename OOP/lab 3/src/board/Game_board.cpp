#include "includes/Game_board.h"

#include <cstdlib>

using namespace std;

Game_board::Game_board(size_t width, size_t height) {
	if (width < kMinSize) {
		width = kMinSize;
	}
	if (width > kMaxSize) {
		width = kMaxSize;
	}
	if (height < kMinSize) {
		height = kMinSize;
	}
	if (height > kMaxSize) {
		height = kMaxSize;
	}
	width_ = width;
	height_ = height;

	// squares_[y][x] — сначала строка, потом столбец
	squares_.resize(height_);
	for (size_t y = 0; y < height_; y++) {
		squares_[y].resize(width_);
	}
}

pair<size_t, size_t> Game_board::get_board_size() const {
	return { width_, height_ };
}

bool Game_board::is_inside(int x, int y) const {
	return x >= 0 && y >= 0 && x < (int) width_ && y < (int) height_;
}

vector<pair<int, int>> Game_board::get_all_coordinates() const {
	vector<pair<int, int>> result;
	for (size_t y = 0; y < height_; y++) {
		for (size_t x = 0; x < width_; x++) {
			result.push_back(make_pair((int) x, (int) y));
		}
	}
	return result;
}

void Game_board::set_square_speed_level(int x, int y, int level) {
	if (is_inside(x, y)) {
		squares_[y][x].set_speed_level(level);
	}
}

int Game_board::get_square_speed_level(int x, int y) const {
	return is_inside(x, y) ? squares_[y][x].get_speed_level() : 1;
}

void Game_board::set_square_border(int x, int y, bool border) {
	if (is_inside(x, y)) {
		squares_[y][x].set_border(border);
	}
}

bool Game_board::get_square_border(int x, int y) const {
	// всё, что за краем поля, считается стеной
	return is_inside(x, y) ? squares_[y][x].is_border() : true;
}

void Game_board::set_square_visible(int x, int y, bool visible) {
	if (is_inside(x, y)) {
		squares_[y][x].set_visible(visible);
	}
}

bool Game_board::get_square_visible(int x, int y) const {
	return is_inside(x, y) && squares_[y][x].is_visible();
}

void Game_board::set_square_discovered(int x, int y, bool discovered) {
	if (is_inside(x, y)) {
		squares_[y][x].set_discovered(discovered);
	}
}

bool Game_board::get_square_discovered(int x, int y) const {
	return is_inside(x, y) && squares_[y][x].is_discovered();
}

void Game_board::add_square_status(int x, int y, shared_ptr<Robot_status> status) {
	if (is_inside(x, y)) {
		squares_[y][x].add_status(status);
	}
}

const vector<shared_ptr<Robot_status>>& Game_board::get_square_statuses(int x, int y) const {
	return is_inside(x, y) ? squares_[y][x].get_statuses() : no_statuses_;
}

void Game_board::remove_random_square_status(int x, int y) {
	if (!is_inside(x, y) || squares_[y][x].get_statuses().empty()) {
		return;
	}
	squares_[y][x].remove_status(rand() % squares_[y][x].get_statuses().size());
}

void Game_board::clear_square_statuses(int x, int y) {
	if (is_inside(x, y)) {
		squares_[y][x].clear_statuses();
	}
}
