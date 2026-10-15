#include "includes/Map_editor.h"

using namespace std;

Map_editor::Map_editor(int width, int height) : board_(width > 0 ? width : 0, height > 0 ? height : 0) {
}

void Map_editor::add_obstacle(int x, int y) {
	board_.set_square_border(x, y, true);
}

void Map_editor::add_speed_level(int x, int y, int level) {
	board_.set_square_speed_level(x, y, level);
}

void Map_editor::set_player(Robot robot, int x, int y) {
	player_.clear();
	player_.push_back({ robot, x, y });
}

void Map_editor::add_robot(Robot robot, int x, int y) {
	robots_.push_back({ robot, x, y });
}

void Map_editor::add_factory(Robot_factory factory, int x, int y) {
	factories_.push_back({ factory, x, y });
}

Game_session Map_editor::build() const {
	// работаем с копией, чтобы build() можно было вызывать повторно
	Game_board board = board_;
	status_generator_.generate(board);
	Game_session session(board);

	// игрок добавляется первым — он всегда имеет индекс kPlayerIndex = 0
	for (size_t i = 0; i < player_.size(); i++) {
		if (session.is_cell_free(player_[i].x, player_[i].y)) {
			session.add_robot(player_[i].robot, player_[i].x, player_[i].y);
		}
	}
	for (size_t i = 0; i < factories_.size(); i++) {
		const Factory_placement& placement = factories_[i];
		// завод ставим, только если все его клетки свободны
		bool fits = true;
		int size = placement.factory.get_size();
		for (int dy = 0; dy < size; dy++) {
			for (int dx = 0; dx < size; dx++) {
				fits = fits && session.is_cell_free(placement.x + dx, placement.y + dy);
			}
		}
		if (fits) {
			session.add_factory(placement.factory, placement.x, placement.y);
		}
	}
	for (size_t i = 0; i < robots_.size(); i++) {
		if (session.is_cell_free(robots_[i].x, robots_[i].y)) {
			session.add_robot(robots_[i].robot, robots_[i].x, robots_[i].y);
		}
	}
	session.update_visibility();
	return session;
}
