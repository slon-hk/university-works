#include "includes/Game_session.h"

#include <cstdlib>
#include <stdexcept>

using namespace std;

// сколько последних записей хранит журнал
const size_t kLogCapacity = 200;
const size_t kInventoryCapacity = 6;
// энергия, восстанавливаемая в конце каждого хода
const int kEnergyPerTurn = 10;
// сколько очков движения стоит атака/лечение соседа
const int kInteractionCost = 1;
// опыт за разрушение завода
const int kExperienceForFactory = 50;

Game_session::Game_session(Game_board board)
	: board_(board), inventory_(kInventoryCapacity), log_(kLogCapacity), turn_(1), rewarded_rank_(0) {
	log_event("--- Turn 1 ---");
}

Game_session::Unit& Game_session::unit(int index) {
	return units_.at(index);
}

const Game_session::Unit& Game_session::unit(int index) const {
	return units_.at(index);
}

string Game_session::unit_name(int index) const {
	if (is_player(index)) {
		return "Player";
	}
	if (unit(index).robot.get_relation() == Relation::Enemy) {
		return "Enemy #" + to_string(index);
	}
	return "Ally #" + to_string(index);
}

void Game_session::add_robot(Robot robot, int x, int y) {
	if (!is_cell_free(x, y)) {
		throw runtime_error("Robot must be placed on a free passable cell");
	}
	Unit new_unit = { robot, x, y, {} };
	// игрок — первый робот; награды начисляются за ранги выше стартового
	if (units_.empty()) {
		rewarded_rank_ = (int) robot.get_rank();
	}
	// каждый враг получает одну случайную способность
	if (robot.get_relation() == Relation::Enemy) {
		new_unit.abilities.push_back(ability_catalog_.create_enemy_ability());
	}
	units_.push_back(new_unit);
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
	factories_.push_back({ factory, x, y });
}

bool Game_session::move_player(Direction direction) {
	if (units_.empty()) {
		return false;
	}
	return act(kPlayerIndex, direction);
}

bool Game_session::act(int index, Direction direction) {
	Unit& mover = unit(index);
	if (!mover.robot.is_alive()) {
		return false;
	}
	int new_x = mover.x;
	int new_y = mover.y;
	if (direction == Direction::UP) {
		new_y--;
	} else if (direction == Direction::DOWN) {
		new_y++;
	} else if (direction == Direction::LEFT) {
		new_x--;
	} else {
		new_x++;
	}
	// шаг в клетку вражеского завода — атака завода
	int factory_index = get_factory_index(new_x, new_y);
	if (factory_index >= 0) {
		if (!mover.robot.is_enemy_of(factories_[factory_index].factory.get_unit())
			|| !mover.robot.spend_movement_points(kInteractionCost)) {
			return false;
		}
		damage_factory(index, factory_index, (int) mover.robot.get_attack_power());
		return true;
	}

	if (!is_passable(new_x, new_y)) {
		return false;
	}

	// в клетке стоит робот — вместо перемещения взаимодействуем с ним
	int target_index = get_unit_index(new_x, new_y);
	if (target_index >= 0) {
		Robot& target = unit(target_index).robot;
		if (!mover.robot.can_interact_with(target) || !mover.robot.spend_movement_points(kInteractionCost)) {
			return false;
		}
		int health_before = (int) target.get_current_health();
		bool attack = mover.robot.is_enemy_of(target);
		mover.robot.interaction_with_robot(&target);
		if (attack) {
			log_health_loss(unit_name(index) + " hits ", target_index, health_before, (int) mover.robot.get_attack_power());
		} else {
			log_event(unit_name(index) + " heals " + unit_name(target_index) + ": +"
					  + to_string((int) target.get_current_health() - health_before) + " HP");
		}
		return true;
	}

	if (!mover.robot.can_move()) {
		if (is_player(index)) {
			log_event("Player is slowed and can't move");
		}
		return false;
	}

	// стоимость входа ограничиваем скоростью, иначе медленный робот
	// никогда не смог бы зайти на «дорогую» клетку
	int cost = board_.get_square_speed_level(new_x, new_y);
	if (cost > (int) mover.robot.get_speed()) {
		cost = (int) mover.robot.get_speed();
	}
	if (!mover.robot.spend_movement_points(cost)) {
		return false;
	}
	mover.x = new_x;
	mover.y = new_y;
	enter_cell(index);
	if (is_player(index)) {
		update_visibility();
	}
	return true;
}

void Game_session::enter_cell(int index) {
	int x = unit(index).x;
	int y = unit(index).y;
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
	// порядок важен: сначала действуют враги, затем срабатывают статусы
	enemy_controller_.run_turn(*this);
	process_statuses();
	spawn_from_factories();
	restore_robots();
	turn_++;
	log_event("--- Turn " + to_string(turn_) + " ---");
	update_visibility();
}

void Game_session::process_statuses() {
	for (size_t i = 0; i < units_.size(); i++) {
		Robot& robot = units_[i].robot;
		if (!robot.is_alive() || robot.get_status_count() == 0) {
			continue;
		}
		int health_before = (int) robot.get_current_health();
		robot.process_statuses_turn_end();
		if ((int) robot.get_current_health() < health_before) {
			log_health_loss("Burn: ", (int) i, health_before, health_before - (int) robot.get_current_health());
		}
	}
}

void Game_session::spawn_from_factories() {
	for (size_t i = 0; i < factories_.size(); i++) {
		vector<Robot> new_units = factories_[i].factory.produce();
		for (size_t j = 0; j < new_units.size(); j++) {
			int spawn_x;
			int spawn_y;
			if (find_free_cell_around(factories_[i], spawn_x, spawn_y)) {
				add_robot(new_units[j], spawn_x, spawn_y);
				log_event("Factory spawned " + unit_name((int) units_.size() - 1));
			}
		}
	}
}

bool Game_session::find_free_cell_around(const Factory_placement& placement, int& out_x, int& out_y) const {
	// обходим кольцо клеток вокруг квадрата завода
	int size = placement.factory.get_size();
	for (int y = placement.y - 1; y <= placement.y + size; y++) {
		for (int x = placement.x - 1; x <= placement.x + size; x++) {
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
	for (size_t i = 0; i < units_.size(); i++) {
		units_[i].robot.change_energy(kEnergyPerTurn);
		units_[i].robot.restore_movement_points();
	}
}

void Game_session::update_visibility() {
	if (units_.empty()) {
		return;
	}
	int radius = (int) unit(kPlayerIndex).robot.get_vision_radius();
	vector<pair<int, int>> cells = board_.get_all_coordinates();
	for (size_t i = 0; i < cells.size(); i++) {
		int x = cells[i].first;
		int y = cells[i].second;
		bool in_view = is_in_radius(kPlayerIndex, x, y, radius);
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
	for (size_t i = 0; i < units_.size(); i++) {
		if (units_[i].robot.is_alive() && units_[i].robot.get_relation() == Relation::Enemy) {
			return false;
		}
	}
	// разрушенные заводы удаляются, так что любой оставшийся — живой
	for (size_t i = 0; i < factories_.size(); i++) {
		if (factories_[i].factory.get_unit().get_relation() == Relation::Enemy) {
			return false;
		}
	}
	return true;
}

bool Game_session::is_game_over() const {
	return !units_.empty() && !unit(kPlayerIndex).robot.is_alive();
}

const Game_board& Game_session::get_board() const {
	return board_;
}

bool Game_session::is_inside_board(int x, int y) const {
	return board_.is_inside(x, y);
}

bool Game_session::is_factory_cell(int x, int y) const {
	return get_factory_index(x, y) >= 0;
}

bool Game_session::is_passable(int x, int y) const {
	return board_.is_inside(x, y) && !board_.get_square_border(x, y) && !is_factory_cell(x, y);
}

bool Game_session::is_cell_free(int x, int y) const {
	return is_passable(x, y) && get_unit_index(x, y) < 0;
}

bool Game_session::is_in_radius(int index, int x, int y, int radius) const {
	// квадратный радиус (расстояние Чебышёва)
	return abs(x - unit(index).x) <= radius && abs(y - unit(index).y) <= radius;
}

int Game_session::get_robot_count() const {
	return (int) units_.size();
}

const Robot& Game_session::get_robot(int index) const {
	return unit(index).robot;
}

int Game_session::get_robot_x(int index) const {
	return unit(index).x;
}

int Game_session::get_robot_y(int index) const {
	return unit(index).y;
}

int Game_session::get_unit_index(int x, int y) const {
	for (size_t i = 0; i < units_.size(); i++) {
		// погибшие роботы остаются в units_, но клетку не занимают
		if (units_[i].robot.is_alive() && units_[i].x == x && units_[i].y == y) {
			return (int) i;
		}
	}
	return -1;
}

bool Game_session::is_player(int index) const {
	return index == kPlayerIndex;
}

int Game_session::get_factory_count() const {
	return (int) factories_.size();
}

const Robot_factory& Game_session::get_factory(int index) const {
	return factories_.at(index).factory;
}

int Game_session::get_factory_x(int index) const {
	return factories_.at(index).x;
}

int Game_session::get_factory_y(int index) const {
	return factories_.at(index).y;
}

int Game_session::get_factory_index(int x, int y) const {
	for (size_t i = 0; i < factories_.size(); i++) {
		int size = factories_[i].factory.get_size();
		if (x >= factories_[i].x && x < factories_[i].x + size && y >= factories_[i].y && y < factories_[i].y + size) {
			return (int) i;
		}
	}
	return -1;
}

void Game_session::damage_factory(int user_index, int factory_index, int damage) {
	Robot_factory& factory = factories_.at(factory_index).factory;
	int health_before = (int) factory.get_current_health();
	factory.take_damage(damage);
	log_event(unit_name(user_index) + " hits Factory: -"
			  + to_string(health_before - (int) factory.get_current_health()) + " HP");
	if (!factory.is_destroyed()) {
		return;
	}
	log_event("Factory destroyed by " + unit_name(user_index));
	unit(user_index).robot.change_experience(kExperienceForFactory);
	// клетки завода освобождаются
	factories_.erase(factories_.begin() + factory_index);
}

void Game_session::add_ability(int index, shared_ptr<Robot_abilities> ability) {
	if (ability != nullptr) {
		unit(index).abilities.push_back(ability);
	}
}

int Game_session::get_ability_count(int index) const {
	return (int) unit(index).abilities.size();
}

const Robot_abilities& Game_session::get_ability(int robot_index, int ability_index) const {
	return *unit(robot_index).abilities.at(ability_index);
}

int Game_session::find_ability(int robot_index, const string& name) const {
	const vector<shared_ptr<Robot_abilities>>& abilities = unit(robot_index).abilities;
	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilities[i]->get_name() == name) {
			return (int) i;
		}
	}
	return -1;
}

bool Game_session::use_ability(int user_index, int ability_index, int target_x, int target_y) {
	Unit& user = unit(user_index);
	if (ability_index < 0 || ability_index >= (int) user.abilities.size() || !user.robot.is_alive()) {
		return false;
	}
	shared_ptr<Robot_abilities> ability = user.abilities[ability_index];
	if (user.robot.get_current_energy() < ability->get_energy_cost()) {
		if (is_player(user_index)) {
			log_event("Not enough energy for " + ability->get_name()
					  + " (need " + to_string(ability->get_energy_cost()) + ")");
		}
		return false;
	}
	if (!ability->can_use(*this, user_index, target_x, target_y)) {
		if (is_player(user_index)) {
			log_event(ability->get_name() + ": no valid target");
		}
		return false;
	}
	// Overload срывает применение, но энергия при этом не тратится
	if (!user.robot.try_activate_ability()) {
		log_event(unit_name(user_index) + " is overloaded: " + ability->get_name() + " failed");
		return false;
	}
	user.robot.change_energy(-(int) ability->get_energy_cost());
	log_event(unit_name(user_index) + " uses " + ability->get_name());
	ability->use(*this, user_index, target_x, target_y);
	update_visibility();
	return true;
}

void Game_session::heal_robot(int index, int heal_amount) {
	Robot& robot = unit(index).robot;
	int health_before = (int) robot.get_current_health();
	robot.change_health(heal_amount);
	log_event("  " + unit_name(index) + ": +" + to_string((int) robot.get_current_health() - health_before) + " HP");
}

void Game_session::restore_energy(int index, int amount) {
	Robot& robot = unit(index).robot;
	int energy_before = (int) robot.get_current_energy();
	robot.change_energy(amount);
	log_event("  " + unit_name(index) + ": +" + to_string((int) robot.get_current_energy() - energy_before) + " energy");
}

void Game_session::deal_damage(int user_index, int target_index, int damage) {
	Robot& target = unit(target_index).robot;
	int health_before = (int) target.get_current_health();
	unit(user_index).robot.deal_damage(&target, damage);
	log_health_loss("  ", target_index, health_before, damage);
}

void Game_session::log_health_loss(const string& prefix, int target_index, int health_before, int damage) {
	const Robot& target = unit(target_index).robot;
	int health_delta = (int) target.get_current_health() - health_before;
	string text = prefix + unit_name(target_index) + ": " + to_string(health_delta) + " HP";

	// потеряно меньше HP, чем нанесено урона, — значит, часть поглотил щит
	if (target.is_alive() && -health_delta < damage) {
		text += " (shield)";
	}
	log_event(text);
	if (!target.is_alive()) {
		log_event("  " + unit_name(target_index) + " destroyed");
		if (is_player(target_index)) {
			log_event("GAME OVER");
		}
	}
}

void Game_session::move_robot(int index, int x, int y) {
	if (!is_cell_free(x, y)) {
		return;
	}
	unit(index).x = x;
	unit(index).y = y;
	log_event("  " + unit_name(index) + " -> (" + to_string(x) + ", " + to_string(y) + ")");
	enter_cell(index);
}

void Game_session::apply_status(int index, shared_ptr<Robot_status> status) {
	if (status == nullptr || !unit(index).robot.is_alive()) {
		return;
	}
	// статус может сработать мгновенно и сразу истечь (например, подбор предмета) —
	// тогда на робота его не вешаем
	status->on_apply(*this, index);

	if (status->is_expired()) {
		return;
	}
	log_event("  " + unit_name(index) + " gets " + status->get_description());
	unit(index).robot.add_status(status);
}

bool Game_session::has_rank_reward() const {
	if (units_.empty() || is_game_over()) {
		return false;
	}
	return (int) unit(kPlayerIndex).robot.get_rank() > rewarded_rank_;
}

bool Game_session::learn_ability(shared_ptr<Robot_abilities> ability) {
	if (!has_rank_reward() || ability == nullptr || find_ability(kPlayerIndex, ability->get_name()) >= 0) {
		return false;
	}
	add_ability(kPlayerIndex, ability);
	rewarded_rank_++;
	log_event("Rank up: learned " + ability->get_name());
	return true;
}

bool Game_session::upgrade_ability(int ability_index) {
	if (!has_rank_reward() || ability_index < 0 || ability_index >= get_ability_count(kPlayerIndex)) {
		return false;
	}
	Robot_abilities& ability = *unit(kPlayerIndex).abilities[ability_index];
	string description = ability.get_upgrade_description();
	ability.upgrade();
	rewarded_rank_++;
	log_event("Rank up: " + ability.get_name() + " " + description);
	return true;
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
		log_event(inventory_.get_item(slot).get_name() + ": can't use now");
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

void Game_session::log_event(const string& text) {
	log_.add(text);
}

const Event_log& Game_session::get_log() const {
	return log_;
}
