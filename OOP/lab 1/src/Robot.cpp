#include "Robot.h"

// enemy передалать на enum
using namespace std;

const size_t kMaxStat = 100000; // 10^5

const int kHealthPerRank = 20;
const int kEnergyPerRank = 10;
const int kAttackPerRank = 5;
const int kExperienceStep = 50;
const int kExperienceForKill = 50;

size_t Robot::clamp_add(size_t value, int delta, size_t lo, size_t hi) {
    int result = (value) + delta;
    if (result < lo) {
        result = lo;
    }
    if (result > hi) {
        result = hi;
    }
    return result;
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
    int enemy
) {
    this->max_health    = clamp_add(0, max_health, 0, kMaxStat);
    this->max_energy    = clamp_add(0, max_energy, 0, kMaxStat);
    this->current_health = clamp_add(0, current_health, 0, this->max_health);
    this->current_energy = clamp_add(0, current_energy, 0, this->max_energy);
    this->attack_power  = clamp_add(0, attack_power, 0, kMaxStat);
    this->current_experience       = clamp_add(0, current_experience, 0, kMaxStat);
    this->experience_to_next_rank  = clamp_add(0, experience_to_next_rank, 0, kMaxStat);
    this->rank          = clamp_add(0, rank, 0, kMaxStat);
    this->vision_radius = clamp_add(0, vision_radius, 0, kMaxStat);
    this->heal_power    = clamp_add(0, heal_power, 0, kMaxStat);
    this->enemy = enemy;
}

void Robot::change_health(int delta) {
    current_health = clamp_add(current_health, delta, 0, max_health);
}

void Robot::change_energy(int delta) {
    current_energy = clamp_add(current_energy, delta, 0, max_energy);
}

void Robot::change_experience(int delta) {
    current_experience = clamp_add(current_experience, delta, 0, kMaxStat);
    rank_up_if_ready();
}

void Robot::rank_up_if_ready() {
    while (experience_to_next_rank > 0 && current_experience >= experience_to_next_rank) {
        current_experience -= experience_to_next_rank;
        rank         = clamp_add(rank, 1, 0, kMaxStat);
        max_health   = clamp_add(max_health, kHealthPerRank, 0, kMaxStat);
        max_energy   = clamp_add(max_energy, kEnergyPerRank, 0, kMaxStat);
        attack_power = clamp_add(attack_power, kAttackPerRank, 0, kMaxStat);
        current_health = max_health;
        experience_to_next_rank = clamp_add(experience_to_next_rank, kExperienceStep, 0, kMaxStat);
    }
}

void Robot::change_attack_power(int delta) {
    attack_power = clamp_add(attack_power, delta, 0, kMaxStat);
}

void Robot::change_vision_radius(int delta) {
    vision_radius = clamp_add(vision_radius, delta, 0, kMaxStat);
}

void Robot::change_rank(int delta) {
    rank = clamp_add(rank, delta, 0, kMaxStat);
}

void Robot::change_experience_to_next_rank(int delta) {
    experience_to_next_rank = clamp_add(experience_to_next_rank, delta, 0, kMaxStat);
}

void Robot::change_max_health(int delta) {
    max_health = clamp_add(max_health, delta, 0, kMaxStat);
    current_health = clamp_add(current_health, 0, 0, max_health);
}

void Robot::change_max_energy(int delta) {
    max_energy = clamp_add(max_energy, delta, 0, kMaxStat);
    current_energy = clamp_add(current_energy, 0, 0, max_energy);
}

void Robot::change_heal_power(int delta) {
    heal_power = clamp_add(heal_power, delta, 0, kMaxStat);
}

void Robot::interaction_with_robot(Robot *robot) {
    if (robot == nullptr) {
        return;
    }
    if (robot->enemy < 0) {
        robot->change_health(-(int) this->attack_power);

        if (!robot->is_alive()) {
            change_experience(kExperienceForKill);
        }
    } else if (robot->enemy > 0) {
        robot->change_health((int) this->heal_power);
    }
}

size_t Robot::get_current_health() const { return current_health; }
size_t Robot::get_max_health() const { return max_health; }
size_t Robot::get_attack_power() const { return attack_power; }
size_t Robot::get_current_energy() const { return current_energy; }
size_t Robot::get_max_energy() const { return max_energy; }
size_t Robot::get_current_experience() const { return current_experience; }
size_t Robot::get_experience_to_next_rank() const { return experience_to_next_rank; }
size_t Robot::get_rank() const { return rank; }
size_t Robot::get_vision_radius() const { return vision_radius; }
size_t Robot::get_heal_power() const { return heal_power; }
int Robot::get_enemy() const { return enemy; }
bool Robot::is_alive() const { return current_health > 0; }
