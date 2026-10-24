#include "includes/Game_session.h"

#include <cstdlib>
#include <stdexcept>

#include "../board/includes/Cell_status_generator.h"
#include "../board/includes/Direction_offset.h"

using namespace std;

// энергия, восстанавливаемая в конце каждого хода
const int kEnergyPerTurn = 10;
// сколько очков движения стоит атака/лечение соседа
const int kInteractionCost = 1;
// опыт за разрушение завода
const int kExperienceForFactory = 50;

Game_session::Game_session(Game_board board)
	: board_(board), inventory_(Player_state::kInventoryCapacity), turn_(1), player_used_ability_(false) {
	log_event("--- Turn 1 ---");
}

void Game_session::add_robot(Robot robot, int x, int y) {
	if (!is_cell_free(x, y)) {
		throw runtime_error("Robot must be placed on a free passable cell");
	}
	// игрок — первый робот; награды начисляются за ранги выше стартового
	if (robots_.is_empty()) {
		rewards_.set_rewarded_rank((int) robot.get_rank());
	}
	vector<shared_ptr<Robot_abilities>> abilities;
	// каждый враг получает одну случайную способность
	if (robot.get_relation() == Relation::Enemy) {
		abilities.push_back(ability_catalog_.create_enemy_ability());
	}
	robots_.add(robot, x, y, abilities);
}

void Game_session::restore_robot(Robot robot, int x, int y, vector<shared_ptr<Robot_abilities>> abilities) {
	if (!is_inside_board(x, y) || (robot.is_alive() && !is_cell_free(x, y))) {
		throw runtime_error("Robot must be placed on a free passable cell");
	}
	robots_.add(robot, x, y, abilities);
}

void Game_session::restore_progress(int turn, int rewarded_rank, bool player_used_ability) {
	turn_ = turn > 0 ? turn : 1;
	rewards_.set_rewarded_rank(rewarded_rank);
	player_used_ability_ = player_used_ability;
}

void Game_session::set_difficulty(const Difficulty& difficulty) {
	difficulty_ = difficulty;
}

const Difficulty& Game_session::get_difficulty() const {
	return difficulty_;
}

bool Game_session::is_ability_limit_reached(int index) const {
	return robots_.is_player(index) && player_used_ability_
		&& difficulty_.has_modifier(Modifier::Single_ability_per_turn);
}

bool Game_session::has_player_used_ability() const {
	return player_used_ability_;
}

void Game_session::restore_inventory(const Inventory& inventory) {
	inventory_ = inventory;
}

void Game_session::restore_log(const vector<string>& entries) {
	reporter_.restore_log(entries);
}

void Game_session::add_factory(Robot_factory factory, int x, int y) {
	int size = factory.get_size();
	for (int dy = 0; dy < size; dy++) {
		for (int dx = 0; dx < size; dx++) {
			if (!is_cell_free(x + dx, y + dy)) {
				throw runtime_error("Factory must be placed on free passable cells");
			}
		}
	}
	for (int dy = 0; dy < size; dy++) {
		for (int dx = 0; dx < size; dx++) {
			// под заводом не должно оставаться статусов
			board_.clear_square_statuses(x + dx, y + dy);
		}
	}
	factories_.add(factory, x, y);
}

bool Game_session::move_player(Direction direction) {
	if (robots_.is_empty()) {
		return false;
	}
	return act(kPlayerIndex, direction);
}

bool Game_session::act(int index, Direction direction) {
	if (!robots_.get_robot(index).is_alive()) {
		return false;
	}
	Direction_offset offset(direction);
	int new_x = robots_.get_x(index) + offset.get_dx();
	int new_y = robots_.get_y(index) + offset.get_dy();
	string target_cell = "(" + to_string(new_x) + ", " + to_string(new_y) + ")";
	if (!is_inside_board(new_x, new_y)) {
		if (robots_.is_player(index)) {
			log_error("Can't move outside the board to " + target_cell);
		}
		return false;
	}
	int factory_index = factories_.find_at(new_x, new_y);
	if (factory_index >= 0) {
		return attack_factory(index, factory_index, target_cell);
	}
	if (!is_passable(new_x, new_y)) {
		if (robots_.is_player(index)) {
			log_error("Cell " + target_cell + " is impassable");
		}
		return false;
	}
	// в клетке стоит робот — вместо перемещения взаимодействуем с ним
	int target_index = robots_.find_at(new_x, new_y);
	if (target_index >= 0) {
		return interact(index, target_index);
	}
	return step(index, new_x, new_y, target_cell);
}

bool Game_session::attack_factory(int index, int factory_index, const string& target_cell) {
	Robot& mover = robots_.get_robot(index);
	bool player = robots_.is_player(index);
	if (!mover.is_enemy_of(factories_.get_factory(factory_index).get_unit())) {
		if (player) {
			log_error("Can't enter an allied factory at " + target_cell);
		}
		return false;
	}
	if (!mover.spend_movement_points(kInteractionCost)) {
		if (player) {
			log_error("Not enough movement points to attack the factory");
		}
		return false;
	}
	damage_factory(index, factory_index, (int) mover.get_attack_power());
	return true;
}

bool Game_session::interact(int index, int target_index) {
	Robot& mover = robots_.get_robot(index);
	Robot& target = robots_.get_robot(target_index);
	bool player = robots_.is_player(index);
	if (!mover.can_interact_with(target)) {
		if (player) {
			log_error("Can't interact with " + robots_.get_name(target_index));
		}
		return false;
	}
	if (!mover.spend_movement_points(kInteractionCost)) {
		if (player) {
			log_error("Not enough movement points to interact with " + robots_.get_name(target_index));
		}
		return false;
	}
	int health_before = (int) target.get_current_health();
	bool attack = mover.is_enemy_of(target);
	size_t rank_before = mover.get_rank();
	mover.interaction_with_robot(&target);
	report_rank_up(index, rank_before);
	if (attack) {
		log_health_loss(robots_.get_name(index) + " hits ", target_index, health_before,
						(int) mover.get_attack_power());
	} else {
		log_event(robots_.get_name(index) + " heals " + robots_.get_name(target_index) + ": +"
				  + to_string((int) target.get_current_health() - health_before) + " HP");
	}
	return true;
}

bool Game_session::step(int index, int x, int y, const string& target_cell) {
	Robot& mover = robots_.get_robot(index);
	bool player = robots_.is_player(index);
	if (!mover.can_move()) {
		if (player) {
			log_error("Player is slowed and can't move");
		}
		return false;
	}
	// стоимость входа ограничиваем скоростью, иначе медленный робот
	// никогда не смог бы зайти на «дорогую» клетку
	int cost = board_.get_square_speed_level(x, y);
	if (cost > (int) mover.get_speed()) {
		cost = (int) mover.get_speed();
	}
	if (!mover.spend_movement_points(cost)) {
		if (player) {
			log_error("Not enough movement points to enter " + target_cell + " (cost " + to_string(cost) + ")");
		}
		return false;
	}
	trace_event(robots_.get_name(index) + " moves from (" + to_string(robots_.get_x(index)) + ", "
				+ to_string(robots_.get_y(index)) + ") to " + target_cell);
	robots_.move_to(index, x, y);
	enter_cell(index);
	if (player) {
		update_visibility();
	}
	return true;
}

void Game_session::enter_cell(int index) {
	int x = robots_.get_x(index);
	int y = robots_.get_y(index);
	const vector<shared_ptr<Robot_status>>& statuses = board_.get_square_statuses(x, y);
	if (statuses.empty()) {
		return;
	}
	for (size_t i = 0; i < statuses.size(); i++) {
		apply_status(index, statuses[i]->clone());
	}
	// робот получает копии всех статусов клетки, а с самой клетки
	// исчезает один случайный
	board_.remove_random_square_status(x, y);
}

void Game_session::end_turn() {
	if (is_game_over()) {
		return;
	}
	trace_event("Player ends turn " + to_string(turn_) + ", other robots act");
	// порядок важен: сначала действуют враги, затем срабатывают статусы
	enemy_controller_.run_turn(*this);
	process_statuses();
	spawn_from_factories();
	restore_robots();
	turn_++;
	player_used_ability_ = false;
	log_event("--- Turn " + to_string(turn_) + " ---");
	if (difficulty_.has_modifier(Modifier::Random_status_each_turn)) {
		apply_random_status_to_player();
	}
	update_visibility();
}

void Game_session::apply_random_status_to_player() {
	if (is_game_over()) {
		return;
	}
	Cell_status_generator generator;
	shared_ptr<Robot_status> status = generator.create_status((Cell_status_type) (rand() % kCellStatusTypeCount));
	log_event("Modifier: random status " + status->get_name());
	apply_status(kPlayerIndex, status);
}

void Game_session::process_statuses() {
	for (int i = 0; i < robots_.get_count(); i++) {
		Robot& robot = robots_.get_robot(i);
		if (!robot.is_alive() || robot.get_status_count() == 0) {
			continue;
		}
		int health_before = (int) robot.get_current_health();
		robot.process_statuses_turn_end();
		if ((int) robot.get_current_health() < health_before) {
			log_health_loss("Burn: ", i, health_before, health_before - (int) robot.get_current_health());
		}
	}
}

void Game_session::spawn_from_factories() {
	for (int i = 0; i < factories_.get_count(); i++) {
		vector<Robot> new_units = factories_.get_factory(i).produce();
		for (size_t j = 0; j < new_units.size(); j++) {
			int spawn_x;
			int spawn_y;
			if (find_free_cell_around(i, spawn_x, spawn_y)) {
				add_robot(new_units[j], spawn_x, spawn_y);
				log_event("Factory spawned " + robots_.get_name(robots_.get_count() - 1));
			}
		}
	}
}

bool Game_session::find_free_cell_around(int factory_index, int& out_x, int& out_y) const {
	// обходим кольцо клеток вокруг квадрата завода
	int size = factories_.get_factory(factory_index).get_size();
	int left = factories_.get_x(factory_index);
	int top = factories_.get_y(factory_index);
	for (int y = top - 1; y <= top + size; y++) {
		for (int x = left - 1; x <= left + size; x++) {
			if (is_cell_free(x, y)) {
				out_x = x;
				out_y = y;
				return true;
			}
		}
	}
	return false;
}

void Game_session::restore_robots() {
	for (int i = 0; i < robots_.get_count(); i++) {
		robots_.get_robot(i).change_energy(kEnergyPerTurn);
		robots_.get_robot(i).restore_movement_points();
	}
}

void Game_session::update_visibility() {
	if (robots_.is_empty()) {
		return;
	}
	int radius = (int) robots_.get_robot(kPlayerIndex).get_vision_radius();
	vector<pair<int, int>> cells = board_.get_all_coordinates();
	for (size_t i = 0; i < cells.size(); i++) {
		int x = cells[i].first;
		int y = cells[i].second;
		bool in_view = robots_.is_in_radius(kPlayerIndex, x, y, radius);
		board_.set_square_visible(x, y, in_view);
		// увиденные препятствия запоминаются и остаются видны в тумане
		if (in_view && board_.get_square_border(x, y)) {
			board_.set_square_discovered(x, y, true);
		}
	}
}

int Game_session::get_turn() const {
	return turn_;
}

bool Game_session::is_level_complete() const {
	return !robots_.has_living_enemies() && !factories_.has_enemy_factories();
}

bool Game_session::is_game_over() const {
	return !robots_.is_empty() && !robots_.get_robot(kPlayerIndex).is_alive();
}

const Game_board& Game_session::get_board() const {
	return board_;
}

const Robot_roster& Game_session::get_robots() const {
	return robots_;
}

const Factory_roster& Game_session::get_factories() const {
	return factories_;
}

bool Game_session::is_inside_board(int x, int y) const {
	return board_.is_inside(x, y);
}

bool Game_session::is_passable(int x, int y) const {
	return board_.is_inside(x, y) && !board_.get_square_border(x, y) && factories_.find_at(x, y) < 0;
}

bool Game_session::is_cell_free(int x, int y) const {
	return is_passable(x, y) && robots_.find_at(x, y) < 0;
}

void Game_session::damage_factory(int user_index, int factory_index, int damage) {
	Robot_factory& factory = factories_.get_factory(factory_index);
	int health_before = (int) factory.get_current_health();
	factory.take_damage(damage);
	log_event(robots_.get_name(user_index) + " hits Factory: -"
			  + to_string(health_before - (int) factory.get_current_health()) + " HP");
	if (!factory.is_destroyed()) {
		return;
	}
	log_event("Factory destroyed by " + robots_.get_name(user_index));
	size_t rank_before = robots_.get_robot(user_index).get_rank();
	robots_.get_robot(user_index).change_experience(kExperienceForFactory);
	report_rank_up(user_index, rank_before);
	// клетки завода освобождаются
	factories_.remove(factory_index);
}

void Game_session::add_ability(int index, shared_ptr<Robot_abilities> ability) {
	robots_.add_ability(index, ability);
}

bool Game_session::use_ability(int user_index, int ability_index, int target_x, int target_y) {
	Robot& user = robots_.get_robot(user_index);
	bool player = robots_.is_player(user_index);
	if (ability_index < 0 || ability_index >= robots_.get_ability_count(user_index) || !user.is_alive()) {
		return false;
	}
	shared_ptr<Robot_abilities> ability = robots_.share_ability(user_index, ability_index);
	if (is_ability_limit_reached(user_index)) {
		log_error("Only one ability per turn");
		return false;
	}
	if (user.get_current_energy() < ability->get_energy_cost()) {
		if (player) {
			log_error("Not enough energy for " + ability->get_name()
					  + " (need " + to_string(ability->get_energy_cost()) + ")");
		}
		return false;
	}
	if (!ability->can_use(*this, user_index, target_x, target_y)) {
		if (player) {
			log_error(ability->get_name() + ": no valid target at (" + to_string(target_x) + ", "
					  + to_string(target_y) + ")");
		}
		return false;
	}
	// Overload срывает применение, но энергия при этом не тратится
	if (!user.try_activate_ability()) {
		log_event(robots_.get_name(user_index) + " is overloaded: " + ability->get_name() + " failed");
		return false;
	}
	user.change_energy(-(int) ability->get_energy_cost());
	log_event(robots_.get_name(user_index) + " uses " + ability->get_name());
	trace_event("  target cell (" + to_string(target_x) + ", " + to_string(target_y) + "), energy left "
				+ to_string(user.get_current_energy()));
	size_t rank_before = user.get_rank();
	if (player) {
		player_used_ability_ = true;
	}
	ability->use(*this, user_index, target_x, target_y);
	report_rank_up(user_index, rank_before);
	update_visibility();
	return true;
}

void Game_session::heal_robot(int index, int heal_amount) {
	Robot& robot = robots_.get_robot(index);
	int health_before = (int) robot.get_current_health();
	robot.change_health(heal_amount);
	log_event("  " + robots_.get_name(index) + ": +" + to_string((int) robot.get_current_health() - health_before)
			  + " HP");
}

void Game_session::restore_energy(int index, int amount) {
	Robot& robot = robots_.get_robot(index);
	int energy_before = (int) robot.get_current_energy();
	robot.change_energy(amount);
	log_event("  " + robots_.get_name(index) + ": +" + to_string((int) robot.get_current_energy() - energy_before)
			  + " energy");
}

void Game_session::deal_damage(int user_index, int target_index, int damage) {
	Robot& target = robots_.get_robot(target_index);
	int health_before = (int) target.get_current_health();
	robots_.get_robot(user_index).deal_damage(&target, damage);
	log_health_loss("  ", target_index, health_before, damage);
}

void Game_session::log_health_loss(const string& prefix, int target_index, int health_before, int damage) {
	const Robot& target = robots_.get_robot(target_index);
	int health_delta = (int) target.get_current_health() - health_before;
	string text = prefix + robots_.get_name(target_index) + ": " + to_string(health_delta) + " HP";

	// потеряно меньше HP, чем нанесено урона, — значит, часть поглотил щит
	if (target.is_alive() && -health_delta < damage) {
		text += " (shield)";
	}
	log_event(text);
	if (!target.is_alive()) {
		log_event("  " + robots_.get_name(target_index) + " destroyed");
		if (robots_.is_player(target_index)) {
			log_event("GAME OVER");
		}
	}
}

void Game_session::move_robot(int index, int x, int y) {
	if (!is_cell_free(x, y)) {
		return;
	}
	robots_.move_to(index, x, y);
	log_event("  " + robots_.get_name(index) + " -> (" + to_string(x) + ", " + to_string(y) + ")");
	enter_cell(index);
}

void Game_session::apply_status(int index, shared_ptr<Robot_status> status) {
	if (status == nullptr || !robots_.get_robot(index).is_alive()) {
		return;
	}
	// статус может сработать мгновенно и сразу истечь (например, подбор предмета) —
	// тогда на робота его не вешаем
	status->on_apply(*this, index);

	if (status->is_expired()) {
		return;
	}
	log_event("  " + robots_.get_name(index) + " gets " + status->get_description());
	robots_.get_robot(index).add_status(status);
}

int Game_session::get_rewarded_rank() const {
	return rewards_.get_rewarded_rank();
}

bool Game_session::has_rank_reward() const {
	return rewards_.is_available(robots_, difficulty_);
}

bool Game_session::learn_ability(shared_ptr<Robot_abilities> ability) {
	return rewards_.learn(robots_, ability, difficulty_, reporter_);
}

bool Game_session::upgrade_ability(int ability_index) {
	return rewards_.upgrade(robots_, ability_index, difficulty_, reporter_);
}

bool Game_session::add_item(shared_ptr<Item> item) {
	if (item == nullptr) {
		return false;
	}
	if (!inventory_.add_item(item)) {
		log_event("Inventory is full: " + item->get_name() + " lost");
		return false;
	}
	log_event("Player picks up " + item->get_name());
	return true;
}

bool Game_session::use_item(int slot) {
	if (slot < 0 || slot >= inventory_.get_size() || is_game_over()) {
		return false;
	}
	if (!inventory_.get_item(slot).can_use(*this, kPlayerIndex)) {
		log_error(inventory_.get_item(slot).get_name() + ": can't use now");
		return false;
	}
	// предмет одноразовый: убираем из инвентаря и применяем
	shared_ptr<Item> item = inventory_.take_item(slot);
	log_event("Player uses " + item->get_name());
	item->use(*this, kPlayerIndex);
	return true;
}

const Inventory& Game_session::get_inventory() const {
	return inventory_;
}

Player_state Game_session::get_player_state() const {
	return Player_state(robots_.get_robot(kPlayerIndex), robots_.get_abilities(kPlayerIndex), inventory_);
}

void Game_session::set_player_state(const Player_state& state) {
	if (robots_.is_empty()) {
		throw runtime_error("Player must be placed before setting its state");
	}
	// координаты остаются от нового уровня, всё остальное — из state
	robots_.get_robot(kPlayerIndex) = state.get_robot();
	robots_.set_abilities(kPlayerIndex, state.get_abilities());
	inventory_ = state.get_inventory();
	// награды за уже достигнутые ранги игрок получил на прошлых уровнях
	rewards_.set_rewarded_rank((int) state.get_robot().get_rank());
	// у перенесённого робота может быть другой радиус обзора
	update_visibility();
}

void Game_session::report_rank_up(int index, size_t rank_before) {
	size_t rank = robots_.get_robot(index).get_rank();
	if (rank > rank_before) {
		log_event(robots_.get_name(index) + " reached rank " + to_string(rank));
	}
}

void Game_session::log_event(const string& text) {
	reporter_.info(text);
}

void Game_session::log_error(const string& text) {
	reporter_.error(text);
}

void Game_session::trace_event(const string& text) const {
	reporter_.trace(text);
}

void Game_session::set_event_listener(Game_event_listener* listener) {
	reporter_.set_listener(listener);
}

const Event_log& Game_session::get_log() const {
	return reporter_.get_log();
}
