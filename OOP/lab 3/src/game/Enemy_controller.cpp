#include "includes/Enemy_controller.h"

#include <cstdlib>

#include "includes/Game_session.h"

using namespace std;


void Enemy_controller::run_turn(Game_session& session) const {

	int count = session.get_robots().get_count();
	for (int i = 0; i < count; i++) {
		if (session.get_robots().is_player(i) || !session.get_robots().get_robot(i).is_alive()) {
			continue;
		}
		// игрок мог погибнуть от предыдущего врага
		if (session.is_game_over()) {
			return;
		}

		use_abilities(session, i);
		act(session, i);
	}
}

bool Enemy_controller::sees_player(const Game_session& session, int index) const {
	if (session.is_game_over()) {
		return false;
	}
	const Robot& robot = session.get_robots().get_robot(index);
	int player_x = session.get_robots().get_x(Game_session::kPlayerIndex);
	int player_y = session.get_robots().get_y(Game_session::kPlayerIndex);
	return robot.get_relation() == Relation::Enemy
		&& session.get_robots().is_in_radius(index, player_x, player_y, (int) robot.get_vision_radius());
}

void Enemy_controller::use_abilities(Game_session& session, int index) const {
	for (int i = 0; i < session.get_robots().get_ability_count(index); i++) {
		// способности с целью у врагов — это телепорт
		if (session.get_robots().get_ability(index, i).needs_target()) {
			teleport_to_player(session, index, i);
			continue;
		}

		const Robot& robot = session.get_robots().get_robot(index);
		// остальные (лечение) применяем, пока не восстановим HP или не кончится энергия
		while (robot.get_current_health() < robot.get_max_health()) {
			bool used = session.use_ability(index, i, session.get_robots().get_x(index), session.get_robots().get_y(index));
			if (!used) {
				break;
			}
		}
	}
}

void Enemy_controller::teleport_to_player(Game_session& session, int index, int ability_index) const {
	if (!sees_player(session, index)) {
		return;
	}
	int player_x = session.get_robots().get_x(Game_session::kPlayerIndex);
	int player_y = session.get_robots().get_y(Game_session::kPlayerIndex);
	// уже стоим рядом с игроком — телепорт не нужен
	if (session.get_robots().is_in_radius(index, player_x, player_y, 1)) {
		return;
	}
	const Robot_abilities& ability = session.get_robots().get_ability(index, ability_index);
	// пробуем клетки сверху, снизу, слева и справа от игрока
	int offset_x[4] = { 0, 0, -1, 1 };
	int offset_y[4] = { -1, 1, 0, 0 };
	for (int k = 0; k < 4; k++) {
		int target_x = player_x + offset_x[k];
		int target_y = player_y + offset_y[k];
		if (ability.can_use(session, index, target_x, target_y)) {
			session.use_ability(index, ability_index, target_x, target_y);
			return;
		}
	}
}

void Enemy_controller::act(Game_session& session, int index) const {
	if (!sees_player(session, index)) {
		step_randomly(session, index);
		return;
	}

	// идём к игроку, пока есть очки движения; атака тоже тратит очки
	while (session.get_robots().get_robot(index).get_movement_points() > 0 && !session.is_game_over()) {
		if (!step_towards_player(session, index)) {
			return;
		}
	}
}

bool Enemy_controller::step_towards_player(Game_session& session, int index) const {
	int dx = session.get_robots().get_x(Game_session::kPlayerIndex) - session.get_robots().get_x(index);
	int dy = session.get_robots().get_y(Game_session::kPlayerIndex) - session.get_robots().get_y(index);
	Direction horizontal = dx > 0 ? Direction::RIGHT : Direction::LEFT;
	Direction vertical = dy > 0 ? Direction::DOWN : Direction::UP;

	// сначала сокращаем большую разницу координат, при неудаче пробуем другую ось
	bool horizontal_first = abs(dx) >= abs(dy);
	Direction first = horizontal_first ? horizontal : vertical;
	Direction second = horizontal_first ? vertical : horizontal;
	bool has_second = horizontal_first ? dy != 0 : dx != 0;

	if (session.act(index, first)) {
		return true;
	}
	if (has_second && session.act(index, second)) {
		return true;
	}
	// обе стороны заблокированы — пробуем обойти случайным шагом
	return step_randomly(session, index);
}

bool Enemy_controller::step_randomly(Game_session& session, int index) const {
	Direction directions[4] = { Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT };
	return session.act(index, directions[rand() % 4]);
}
