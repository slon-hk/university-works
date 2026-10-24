#include "includes/Map_editor.h"

using namespace std;

// сколько очков движения стоит войти в «вязкую» клетку
const int kSlowCellCost = 3;

Map_editor::Map_editor(int width, int height) : board_(width > 0 ? width : 0, height > 0 ? height : 0) {
}

void Map_editor::add_obstacle(int x, int y) {
	board_.set_square_border(x, y, true);
}

void Map_editor::add_speed_level(int x, int y, int level) {
	board_.set_square_speed_level(x, y, level);
}

void Map_editor::add_slow_cell(int x, int y) {
	board_.set_square_speed_level(x, y, kSlowCellCost);
}

void Map_editor::add_cell_status(int x, int y, shared_ptr<Robot_status> status) {
	board_.add_square_status(x, y, status);
}

void Map_editor::set_status_chances(int one_status_chance, int two_status_chance) {
	status_generator_ = Cell_status_generator(one_status_chance, two_status_chance);
}

void Map_editor::set_player(Robot robot, int x, int y) {
	player_.clear();
	player_.push_back(Robot_unit(robot, x, y, {}));
}

void Map_editor::add_robot(Robot robot, int x, int y) {
	robots_.push_back(Robot_unit(robot, x, y, {}));
}

void Map_editor::add_factory(Robot_factory factory, int x, int y) {
	factories_.push_back(Placed_factory(factory, x, y));
}

Game_session Map_editor::build() const {
	// работаем с копией, чтобы build() можно было вызывать повторно
	Game_board board = board_;
	status_generator_.generate(board);
	Game_session session(board);

	// игрок добавляется первым — он всегда имеет индекс kPlayerIndex = 0
	for (size_t i = 0; i < player_.size(); i++) {
		if (session.is_cell_free(player_[i].get_x(), player_[i].get_y())) {
			session.add_robot(player_[i].get_robot(), player_[i].get_x(), player_[i].get_y());
		}
	}
	for (size_t i = 0; i < factories_.size(); i++) {
		const Placed_factory& placement = factories_[i];
		// завод ставим, только если все его клетки свободны
		bool fits = true;
		int size = placement.get_factory().get_size();
		for (int dy = 0; dy < size; dy++) {
			for (int dx = 0; dx < size; dx++) {
				fits = fits && session.is_cell_free(placement.get_x() + dx, placement.get_y() + dy);
			}
		}
		if (fits) {
			session.add_factory(placement.get_factory(), placement.get_x(), placement.get_y());
		}
	}
	for (size_t i = 0; i < robots_.size(); i++) {
		if (session.is_cell_free(robots_[i].get_x(), robots_[i].get_y())) {
			session.add_robot(robots_[i].get_robot(), robots_[i].get_x(), robots_[i].get_y());
		}
	}
	session.update_visibility();
	return session;
}
