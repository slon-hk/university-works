#ifndef OOP_PROJECT_ROBOT_H
#define OOP_PROJECT_ROBOT_H

#include <cstddef>


class Robot {
private:
    std::size_t current_health;
    std::size_t max_health;
    std::size_t attack_power;
    std::size_t current_energy;
    std::size_t max_energy;
    std::size_t current_experience;
    std::size_t experience_to_next_rank;
    std::size_t rank;
    std::size_t vision_radius;
    std::size_t heal_power;
    int enemy;


    std::size_t clamp_add(size_t value, int delta, size_t lo, size_t hi);
    void rank_up_if_ready();

public:
    Robot(
        std::size_t current_health,
        std::size_t max_health,
        std::size_t attack_power,
        std::size_t current_energy,
        std::size_t max_energy,
        std::size_t current_experience,
        std::size_t experience_to_next_rank,
        std::size_t rank,
        std::size_t vision_radius,
        std::size_t heal_power,
        int enemy
    );

    void change_health(int delta);
    void change_energy(int delta);
    void change_experience(int delta);
    void change_attack_power(int delta);
    void change_vision_radius(int delta);
    void change_rank(int delta);
    void change_experience_to_next_rank(int delta);
    void change_max_health(int delta);
    void change_max_energy(int delta);
    void change_heal_power(int delta);

    void interaction_with_robot(Robot *robot);

    std::size_t get_current_health() const;
    std::size_t get_max_health() const;
    std::size_t get_attack_power() const;
    std::size_t get_current_energy() const;
    std::size_t get_max_energy() const;
    std::size_t get_current_experience() const;
    std::size_t get_experience_to_next_rank() const;
    std::size_t get_rank() const;
    std::size_t get_vision_radius() const;
    std::size_t get_heal_power() const;
    int get_enemy() const;
    bool is_alive() const;
};

#endif
