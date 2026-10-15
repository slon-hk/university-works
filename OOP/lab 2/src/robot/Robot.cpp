#include "includes/Robot.h"

using namespace std;

// верхняя граница для любой характеристики
const size_t kMaxStat = 100000;

// прибавка характеристик при повышении ранга
const int kHealthPerRank = 20;
const int kEnergyPerRank = 10;
const int kAttackPerRank = 5;
// на сколько растёт порог опыта с каждым рангом
const int kExperienceStep = 50;
// опыт за уничтоженного робота
const int kExperienceForKill = 50;

size_t Robot::clamp_add(size_t value, int delta, size_t lower, size_t upper) const {
	// считаем в long long, чтобы не было переполнения size_t при отрицательном delta
	long long result = (long long) value + delta;
	if (result < (long long) lower) {
		result = (long long) lower;
	}
	if (result > (long long) upper) {
		result = (long long) upper;
	}
	return (size_t) result;
}

Robot::Robot(
	size_t current_health,
	size_t max_health,
	size_t attack_power,
	size_t current_energy,
	size_t max_energy,
	size_t current_experience,
	size_t experience_to_next_rank,
	size_t rank,
	size_t vision_radius,
	size_t heal_power,
	size_t speed,
	Relation relation
) {
	// все значения обрезаются по допустимым границам
	max_health_ = min(max_health, kMaxStat);
	max_energy_ = min(max_energy, kMaxStat);
	current_health_ = min(current_health, max_health_);
	current_energy_ = min(current_energy, max_energy_);
	attack_power_ = min(attack_power, kMaxStat);
	current_experience_ = min(current_experience, kMaxStat);
	experience_to_next_rank_ = min(experience_to_next_rank, kMaxStat);
	rank_ = min(rank, kMaxStat);
	vision_radius_ = min(vision_radius, kMaxStat);
	heal_power_ = min(heal_power, kMaxStat);
	speed_ = min(speed, kMaxStat);
	movement_points_ = speed_;
	relation_ = relation;
}

void Robot::change_health(int delta) {
	current_health_ = clamp_add(current_health_, delta, 0, max_health_);
}

void Robot::change_energy(int delta) {
	current_energy_ = clamp_add(current_energy_, delta, 0, max_energy_);
}

void Robot::change_experience(int delta) {
	current_experience_ = clamp_add(current_experience_, delta, 0, kMaxStat);
	rank_up_if_ready();
}

void Robot::rank_up_if_ready() {
	// опыта может хватить сразу на несколько рангов
	while (experience_to_next_rank_ > 0 && current_experience_ >= experience_to_next_rank_) {
		current_experience_ -= experience_to_next_rank_;
		rank_ = clamp_add(rank_, 1, 0, kMaxStat);
		max_health_ = clamp_add(max_health_, kHealthPerRank, 0, kMaxStat);
		max_energy_ = clamp_add(max_energy_, kEnergyPerRank, 0, kMaxStat);
		attack_power_ = clamp_add(attack_power_, kAttackPerRank, 0, kMaxStat);
		// при повышении ранга здоровье восстанавливается полностью
		current_health_ = max_health_;
		experience_to_next_rank_ = clamp_add(experience_to_next_rank_, kExperienceStep, 0, kMaxStat);
	}
}

void Robot::change_attack_power(int delta) {
	attack_power_ = clamp_add(attack_power_, delta, 0, kMaxStat);
}

void Robot::change_vision_radius(int delta) {
	vision_radius_ = clamp_add(vision_radius_, delta, 0, kMaxStat);
}

void Robot::change_rank(int delta) {
	rank_ = clamp_add(rank_, delta, 0, kMaxStat);
}

void Robot::change_experience_to_next_rank(int delta) {
	experience_to_next_rank_ = clamp_add(experience_to_next_rank_, delta, 0, kMaxStat);
}

void Robot::change_max_health(int delta) {
	max_health_ = clamp_add(max_health_, delta, 0, kMaxStat);
	current_health_ = min(current_health_, max_health_);
}

void Robot::change_max_energy(int delta) {
	max_energy_ = clamp_add(max_energy_, delta, 0, kMaxStat);
	current_energy_ = min(current_energy_, max_energy_);
}

void Robot::change_heal_power(int delta) {
	heal_power_ = clamp_add(heal_power_, delta, 0, kMaxStat);
}

void Robot::change_speed(int delta) {
	speed_ = clamp_add(speed_, delta, 0, kMaxStat);
	// текущие очки движения не могут превышать новую скорость
	movement_points_ = min(movement_points_, speed_);
}

bool Robot::spend_movement_points(int cost) {
	if (cost < 0 || (size_t) cost > movement_points_) {
		return false;
	}
	movement_points_ -= cost;
	return true;
}

void Robot::restore_movement_points() {
	movement_points_ = speed_;
}

bool Robot::can_move() const {
	return !statuses_.blocks_movement();
}

bool Robot::is_enemy_of(const Robot& other) const {
	// игрок (Neutral) и союзники (Ally) — одна сторона, Enemy — другая
	return (relation_ == Relation::Enemy) != (other.relation_ == Relation::Enemy);
}

bool Robot::can_interact_with(const Robot& other) const {
	// лечить союзников может только сторона игрока
	bool heals_ally = relation_ != Relation::Enemy && other.relation_ == Relation::Ally;
	return is_enemy_of(other) || heals_ally;
}

void Robot::interaction_with_robot(Robot* robot) {
	if (robot == nullptr || !can_interact_with(*robot)) {
		return;
	}
	if (is_enemy_of(*robot)) {
		deal_damage(robot, (int) get_attack_power());
	} else {
		robot->change_health((int) heal_power_);
	}
}

void Robot::deal_damage(Robot* robot, int damage) {
	if (robot == nullptr || !robot->is_alive()) {
		return;
	}
	robot->take_damage(damage);
	// если цель погибла от этого удара — получаем опыт
	if (!robot->is_alive()) {
		change_experience(kExperienceForKill);
	}
}

void Robot::take_damage(int damage) {
	// сначала урон проходит через статусы (щит может поглотить часть)
	change_health(-statuses_.absorb_damage(damage));
}

void Robot::add_status(shared_ptr<Robot_status> status) {
	statuses_.add(status);
}

bool Robot::try_activate_ability() {
	return statuses_.try_activate_ability();
}

void Robot::process_statuses_turn_end() {
	int damage = statuses_.process_turn_end();
	if (damage > 0) {
		take_damage(damage);
	}
}

int Robot::get_status_count() const {
	return statuses_.get_count();
}

const Robot_status& Robot::get_status(int index) const {
	return statuses_.get(index);
}

size_t Robot::get_current_health() const {
	return current_health_;
}

size_t Robot::get_max_health() const {
	return max_health_;
}

size_t Robot::get_attack_power() const {
	return attack_power_ + get_attack_bonus();
}

size_t Robot::get_attack_bonus() const {
	int bonus = statuses_.get_attack_bonus();
	return bonus > 0 ? (size_t) bonus : 0;
}

size_t Robot::get_current_energy() const {
	return current_energy_;
}

size_t Robot::get_max_energy() const {
	return max_energy_;
}

size_t Robot::get_current_experience() const {
	return current_experience_;
}

size_t Robot::get_experience_to_next_rank() const {
	return experience_to_next_rank_;
}

size_t Robot::get_rank() const {
	return rank_;
}

size_t Robot::get_vision_radius() const {
	return vision_radius_;
}

size_t Robot::get_heal_power() const {
	return heal_power_;
}

size_t Robot::get_speed() const {
	return speed_;
}

size_t Robot::get_movement_points() const {
	return movement_points_;
}

Relation Robot::get_relation() const {
	return relation_;
}

bool Robot::is_alive() const {
	return current_health_ > 0;
}
