#ifndef OOP_PROJECT_BOARD_VIEW_H
#define OOP_PROJECT_BOARD_VIEW_H

#include <utility>
#include <vector>

#include "Canvas.h"
#include "Display_mode.h"
#include "Display_mode_selector.h"
#include "../../game/includes/Game_session.h"
#include "Layout.h"
#include "Player_controller.h"
#include "Sprite_catalog.h"

// Отрисовка игрового поля: местность с туманом войны, статусы на клетках,
// заводы, зона действия способности, роботы с подписями и подсветка
// в режимах отображения.
class Board_view {
private:
	Canvas& canvas_;
	const Layout& layout_;
	const Sprite_catalog& sprites_;
	// последняя x-координата и направление взгляда каждого робота —
	// чтобы спрайт поворачивался в сторону движения
	std::vector<int> last_robot_x_;
	std::vector<bool> robot_facing_left_;

	void draw_terrain(const Game_session& session);
	void draw_cell_statuses(const Game_session& session);
	void draw_factories(const Game_session& session);
	void draw_ability_area(const Game_session& session, const Player_controller& controller);
	void draw_robots(const Game_session& session);
	void draw_robot_labels(const Game_session& session, const std::vector<int>& order);
	void draw_highlight(const Game_session& session, const Display_mode_selector& display_mode);

	// индексы видимых живых роботов, отсортированные по y (дальние рисуются раньше)
	std::vector<int> find_visible_robots(const Game_session& session);
	// собирает клетки и цвета для подсветки в текущем режиме отображения
	void find_highlighted_cells(const Game_session& session, Display_mode mode,
								std::vector<std::pair<int, int>>& cells, std::vector<SDL_Color>& colors) const;

public:
	Board_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites);

	// рисует поле целиком (слои снизу вверх)
	void draw(const Game_session& session, const Player_controller& controller,
			  const Display_mode_selector& display_mode);
};

#endif
