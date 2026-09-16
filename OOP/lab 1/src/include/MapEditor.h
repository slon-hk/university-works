#ifndef OOP_PROJECT_MAP_EDITOR_H
#define OOP_PROJECT_MAP_EDITOR_H

#include <vector>
#include <utility>
#include <array>

#include "Robot.h"
#include "GameSession.h"
#include "Robot_factory.h"

class MapEditor {
private:
    struct Placement {
        Robot robot;
        int x;
        int y;
    };

    struct Placement_factory {
        Robot_factory factory;
        int x;
        int y;
    };

    int width_;
    int height_;
    std::vector<std::pair<int, int>> obstacles_;
    std::vector<std::array<int, 3>> speed_level_;
    std::vector<Placement> player_;
    std::vector<Placement> enemies_;
    std::vector<Placement_factory> factory_;

    bool is_inside(int x, int y) const;

public:
    MapEditor(int width, int height);

    void add_obstacle(int x, int y);
    void add_speed_level(int x, int y, int level);
    void set_player(Robot robot, int x, int y);
    void add_enemy(Robot robot, int x, int y);
    void add_factory(Robot_factory factory, int x, int y);

    GameSession build() const;
};

#endif