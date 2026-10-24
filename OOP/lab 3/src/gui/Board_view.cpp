#include "includes/Board_view.h"

#include "includes/Palette.h"

#include <cmath>

using namespace std;

const SDL_Color kFog = { 15, 15, 15, 255 };
const SDL_Color kGridLine = { 0, 0, 0, 35 };
// скорость «пульсации» иконок статусов на клетках
const int kPulsePeriodMs = 250;
// полоска здоровья завода
const float kFactoryBarHeight = 5.0f;
const SDL_Color kFactoryBarBack = { 40, 40, 40, 220 };
const SDL_Color kFactoryBarFill = { 220, 50, 40, 255 };

Board_view::Board_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites)
	: canvas_(canvas), layout_(layout), sprites_(sprites) {
}

void Board_view::draw(const Game_session& session, const Player_controller& controller,
					  const Display_mode_selector& display_mode) {
	draw_terrain(session);
	draw_cell_statuses(session);
	draw_factories(session);
	draw_ability_area(session, controller);
	draw_robots(session);
	draw_highlight(session, display_mode);
}

void Board_view::draw_terrain(const Game_session& session) {
	const Game_board& board = session.get_board();
	vector<pair<int, int>> cells = board.get_all_coordinates();
	SDL_FRect grass_source = sprites_.get_grass_source();

	for (size_t i = 0; i < cells.size(); i++) {
		int x = cells[i].first;
		int y = cells[i].second;
		SDL_FRect cell = layout_.get_cell_rect(x, y);

		bool visible = board.get_square_visible(x, y);
		bool discovered_border = board.get_square_border(x, y) && board.get_square_discovered(x, y);
		// невидимые клетки закрыты туманом, кроме уже обнаруженных препятствий
		if (!visible && !discovered_border) {
			canvas_.fill_rect(cell, kFog);
			continue;
		}

		int level = board.get_square_speed_level(x, y);
		SDL_Color tint = Palette::kWhite;
		// чем дороже проход по клетке, тем более «жёлтой» она выглядит
		if (level > 1) {
			tint.g = (Uint8) max(110, 255 - 15 * level);
			tint.b = (Uint8) max(60, 255 - 25 * level);
		}
		if (!visible) {
			// обнаруженное, но сейчас невидимое препятствие рисуется затемнённым
			tint = { (Uint8) (tint.r / 3), (Uint8) (tint.g / 3), (Uint8) (tint.b / 3), 255 };
		}
		canvas_.draw_image(sprites_.get_grass_tileset(), &grass_source, cell, tint);

		if (board.get_square_border(x, y)) {
			Uint8 shade = visible ? 255 : 110;
			canvas_.draw_image(sprites_.get_rock_sprite(x, y), nullptr, cell, { shade, shade, shade, 255 });
		}
		canvas_.draw_rect(cell, kGridLine);
	}
}

void Board_view::draw_cell_statuses(const Game_session& session) {
	const Game_board& board = session.get_board();
	vector<pair<int, int>> cells = board.get_all_coordinates();
	float cell_size = layout_.get_cell_size();
	float pulse = 1.0f + 0.08f * sinf((float) canvas_.get_animation_step(1) / kPulsePeriodMs);

	for (size_t i = 0; i < cells.size(); i++) {
		int x = cells[i].first;
		int y = cells[i].second;
		if (!board.get_square_visible(x, y)) {
			continue;
		}
		const vector<shared_ptr<Robot_status>>& statuses = board.get_square_statuses(x, y);
		SDL_FRect cell = layout_.get_cell_rect(x, y);
		for (size_t k = 0; k < statuses.size(); k++) {

			// один статус — по центру клетки, два — по диагонали со сдвигом
			float size = (statuses.size() == 1 ? cell_size + 4 : cell_size - 2) * pulse;
			float shift = statuses.size() > 1 ? (k == 0 ? -1.0f : 1.0f) * cell_size / 4 : 0;
			SDL_FRect icon = { cell.x + cell_size / 2 + shift - size / 2, cell.y + cell_size / 2 + shift - size / 2,
							   size, size };

			// для предмета описание статуса — это имя предмета
			if (statuses[k]->get_name() == "Item") {
				sprites_.draw_item_icon(canvas_, statuses[k]->get_description(), icon);
			} else {
				sprites_.draw_status_icon(canvas_, statuses[k]->get_name(), x + y, icon);
			}
		}
	}
}

void Board_view::draw_factories(const Game_session& session) {
	const Game_board& board = session.get_board();
	float cell_size = layout_.get_cell_size();

	for (int i = 0; i < session.get_factories().get_count(); i++) {
		int x = session.get_factories().get_x(i);
		int y = session.get_factories().get_y(i);
		int size = session.get_factories().get_factory(i).get_size();
		bool visible = false;
		for (int dy = 0; dy < size; dy++) {
			for (int dx = 0; dx < size; dx++) {
				visible = visible || board.get_square_visible(x + dx, y + dy);
			}
		}
		if (!visible) {
			continue;
		}

		SDL_FRect building;
		building.w = size * cell_size;
		// сохраняем пропорции спрайта казармы (192x256) и ставим его на нижний край завода
		building.h = building.w * 256.0f / 192.0f;
		building.x = x * cell_size;
		building.y = (y + size) * cell_size - building.h;
		canvas_.draw_image(sprites_.get_factory_sprite(), nullptr, building);

		// полоска здоровья над зданием
		const Robot_factory& factory = session.get_factories().get_factory(i);
		float health_ratio = (float) factory.get_current_health() / (float) factory.get_max_health();
		SDL_FRect bar = { building.x, building.y - kFactoryBarHeight, building.w, kFactoryBarHeight };
		canvas_.fill_rect(bar, kFactoryBarBack);
		bar.w *= health_ratio;
		canvas_.fill_rect(bar, kFactoryBarFill);
	}
}

void Board_view::draw_ability_area(const Game_session& session, const Player_controller& controller) {
	int ability_index = controller.get_selected_ability_index();
	if (ability_index < 0 || session.is_game_over()) {
		return;
	}
	const int player = Game_session::kPlayerIndex;
	const Robot_abilities& ability = session.get_robots().get_ability(player, ability_index);
	int radius = ability.get_radius();

	// зону прицельной способности показываем только в режиме выбора цели
	if (radius <= 0 || (ability.needs_target() && !controller.is_targeting())) {
		return;
	}
	int player_x = session.get_robots().get_x(player);
	int player_y = session.get_robots().get_y(player);
	for (int y = player_y - radius; y <= player_y + radius; y++) {
		for (int x = player_x - radius; x <= player_x + radius; x++) {
			if (!session.is_inside_board(x, y) || (x == player_x && y == player_y)) {
				continue;
			}
			SDL_Color color = { 255, 80, 60, 45 };
			if (controller.is_targeting()) {
				// в режиме прицеливания доступные цели выделяются жёлтым
				color = ability.can_use(session, player, x, y) ? SDL_Color{ 255, 255, 0, 110 } : SDL_Color{ 255, 255, 255, 30 };
			}
			canvas_.fill_rect(layout_.get_cell_rect(x, y), color);
		}
	}
	if (controller.is_targeting()) {
		SDL_FRect cursor = layout_.get_cell_rect(controller.get_cursor_x(), controller.get_cursor_y());
		canvas_.draw_rect(cursor, Palette::kYellow, 3);
	}
}

vector<int> Board_view::find_visible_robots(const Game_session& session) {
	int count = session.get_robots().get_count();
	// появились новые роботы (например, от завода) — расширяем массивы;
	// враги изначально смотрят влево
	for (int i = (int) last_robot_x_.size(); i < count; i++) {
		last_robot_x_.push_back(session.get_robots().get_x(i));
		robot_facing_left_.push_back(!session.get_robots().is_player(i));
	}

	vector<int> order;
	for (int i = 0; i < count; i++) {
		if (!session.get_robots().get_robot(i).is_alive()) {
			continue;
		}
		int x = session.get_robots().get_x(i);
		if (x != last_robot_x_[i]) {
			robot_facing_left_[i] = x < last_robot_x_[i];
			last_robot_x_[i] = x;
		}
		if (session.get_board().get_square_visible(x, session.get_robots().get_y(i))) {
			order.push_back(i);
		}
	}

	// сортировка по y: роботы ниже на экране рисуются поверх тех, что выше
	for (size_t a = 0; a < order.size(); a++) {
		for (size_t b = a + 1; b < order.size(); b++) {
			if (session.get_robots().get_y(order[b]) < session.get_robots().get_y(order[a])) {
				int temp = order[a];
				order[a] = order[b];
				order[b] = temp;
			}
		}
	}
	return order;
}

void Board_view::draw_robots(const Game_session& session) {
	vector<int> order = find_visible_robots(session);
	float cell_size = layout_.get_cell_size();
	float sprite_size = 3 * cell_size;

	for (size_t k = 0; k < order.size(); k++) {
		int i = order[k];
		SDL_FRect cell = layout_.get_cell_rect(session.get_robots().get_x(i), session.get_robots().get_y(i));

		// спрайт в 3 раза больше клетки, центрирован по x и «стоит» на клетке
		SDL_FRect sprite = { cell.x + cell_size / 2 - sprite_size / 2, cell.y + cell_size - sprite_size * 0.7f,
							 sprite_size, sprite_size };
		canvas_.draw_animated(sprites_.get_unit_sprite(session, i), i * 3, sprite, robot_facing_left_[i]);
	}
	draw_robot_labels(session, order);
}

void Board_view::draw_robot_labels(const Game_session& session, const vector<int>& order) {

	float cell_size = layout_.get_cell_size();
	const float icon_size = 10.0f;
	for (size_t k = 0; k < order.size(); k++) {
		int i = order[k];
		const Robot& robot = session.get_robots().get_robot(i);
		SDL_FRect cell = layout_.get_cell_rect(session.get_robots().get_x(i), session.get_robots().get_y(i));
		if (!session.get_robots().is_player(i)) {
			// подпись над роботом: номер, HP / атака
			string label = "#" + to_string(i) + " " + to_string(robot.get_current_health())
				+ "/" + to_string(robot.get_attack_power());
			canvas_.draw_text(label, cell.x - 4, cell.y - cell_size - 6, Palette::kWhite);
		}
		for (int s = 0; s < robot.get_status_count(); s++) {
			SDL_FRect icon = { cell.x + s * icon_size, cell.y + cell_size - icon_size / 2, icon_size, icon_size };
			sprites_.draw_status_icon(canvas_, robot.get_status(s).get_name(), s, icon);
		}
	}
}

void Board_view::find_highlighted_cells(const Game_session& session, Display_mode mode,
										vector<pair<int, int>>& cells, vector<SDL_Color>& colors) const {

	const Game_board& board = session.get_board();
	if (mode == Display_mode::Enemies) {
		for (int i = 0; i < session.get_robots().get_count(); i++) {
			int x = session.get_robots().get_x(i);
			int y = session.get_robots().get_y(i);
			const Robot& robot = session.get_robots().get_robot(i);
			if (robot.is_alive() && robot.get_relation() == Relation::Enemy && board.get_square_visible(x, y)) {
				cells.push_back(make_pair(x, y));
				colors.push_back({ 255, 50, 40, 255 });
			}
		}
		return;
	}
	vector<pair<int, int>> all = board.get_all_coordinates();
	for (size_t i = 0; i < all.size(); i++) {
		int x = all[i].first;
		int y = all[i].second;
		if (mode == Display_mode::Obstacles) {
			bool known = board.get_square_visible(x, y) || board.get_square_discovered(x, y);
			if (known && !session.is_passable(x, y)) {
				cells.push_back(all[i]);
				colors.push_back({ 70, 150, 255, 255 });
			}
		} else if (board.get_square_visible(x, y) && !board.get_square_statuses(x, y).empty()) {
			cells.push_back(all[i]);
			colors.push_back(sprites_.get_status_color(board.get_square_statuses(x, y)[0]->get_name()));
		}
	}
}

void Board_view::draw_highlight(const Game_session& session, const Display_mode_selector& display_mode) {
	Display_mode mode = display_mode.get_mode();
	if (mode == Display_mode::Standard) {
		return;
	}
	vector<pair<int, int>> cells;
	vector<SDL_Color> colors;
	find_highlighted_cells(session, mode, cells, colors);

	SDL_FRect board_rect = { 0, 0, layout_.get_board_width(), layout_.get_board_height() };
	// затемняем всё поле, чтобы подсвеченные клетки выделялись
	canvas_.fill_rect(board_rect, { 0, 0, 0, 140 });
	for (size_t i = 0; i < cells.size(); i++) {
		SDL_FRect cell = layout_.get_cell_rect(cells[i].first, cells[i].second);
		SDL_Color fill = colors[i];
		fill.a = 110;
		canvas_.fill_rect(cell, fill);
		canvas_.draw_rect(cell, colors[i], 2);
	}
	canvas_.draw_title(display_mode.get_name() + ": " + to_string(cells.size()), 8, 6, Palette::kWhite);
}
