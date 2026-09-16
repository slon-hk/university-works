#ifndef OOP_PROJECT_GAME_SESSION_H
#define OOP_PROJECT_GAME_SESSION_H

#include <vector>

#include "Game_board.h"
#include "Robot.h"
#include "Robot_factory.h"
#include "Direction.h"

class GameSession {
private:
    struct Unit {
        Robot robot;
        int x;
        int y;
    };

    struct Fabric {
        Robot_factory fabric;
        int x;
        int y;
    };

    GameBoard board_;
    std::vector<Unit> units_;
    std::vector<Fabric> fabric_;

    bool is_inside_board(int x, int y) const;
    bool is_passable(int x, int y) const;
    Unit* unit_at(int x, int y);
    bool try_action(Unit& mover, Direction direction);

    void run_enemies_turn();
    void regenerate_energy();
    bool find_free_cell_around(int center_x, int center_y, int& out_x, int& out_y);
    void spawn_from_factories();

public:
    GameSession(GameBoard board);

    void add_robot(Robot robot, int x, int y);
    void add_fabric(Robot_factory factory, int x, int y);
    bool move_player(Direction direction);
    void end_turn();

    void update_visibility();

    bool is_level_complete() const;

    int get_robot_count() const;
    int get_robot_x(int index) const;
    int get_robot_y(int index) const;
    int get_robot_enemy(int index) const;
    bool get_robot_alive(int index) const;

    int get_robot_current_health(int index) const;
    int get_robot_max_health(int index) const;
    int get_robot_current_energy(int index) const;
    int get_robot_max_energy(int index) const;
    int get_robot_attack_power(int index) const;
    int get_robot_rank(int index) const;
    int get_robot_current_experience(int index) const;
    int get_robot_experience_to_next_rank(int index) const;

    int get_fabric_count() const;
    int get_fabric_x(int index) const;
    int get_fabric_y(int index) const;

    const GameBoard& get_board() const;
};

#endif