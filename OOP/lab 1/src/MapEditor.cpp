#include "MapEditor.h"

#include "Game_board.h"

using namespace std;

MapEditor::MapEditor(int width, int height) {
    width_ = width;
    height_ = height;
}

bool MapEditor::is_inside(int x, int y) const {
    if (x < 0 || y < 0) {
        return false;
    }
    if (x >= width_ || y >= height_) {
        return false;
    }
    return true;
}

void MapEditor::add_obstacle(int x, int y) {
    if (!is_inside(x, y)) {
        return;
    }
    obstacles_.push_back(make_pair(x, y));
}

void MapEditor::add_speed_level(int x, int y, int level) {
    if (!is_inside(x, y)) {
        return;
    }
    speed_level_.push_back((array<int, 3>{x, y, level}));
}

void MapEditor::set_player(Robot robot, int x, int y) {
    Placement placement = { robot, x, y };
    if (player_.empty()) {
        player_.push_back(placement);
    } else {
        player_[0] = placement;
    }
}

void MapEditor::add_enemy(Robot robot, int x, int y) {
    Placement placement = { robot, x, y };
    enemies_.push_back(placement);
}

void MapEditor::add_factory(Robot_factory factory, int x, int y) {
    Placement_factory placement = {factory, x, y };
    factory_.push_back(placement);
}

GameSession MapEditor::build() const {
    GameBoard board(width_, height_);
    for (size_t i = 0; i < obstacles_.size(); i++) {
        int x = obstacles_[i].first;
        int y = obstacles_[i].second;
        board.set_square_border(x, y, true);
    }

    for (size_t i = 0; i < speed_level_.size(); i++) {
        int x = speed_level_[i][0];
        int y = speed_level_[i][1];
        int level = speed_level_[i][2];
        board.set_square_speed_level(x, y, level);
    }

    GameSession session(board);

    bool has_player = !player_.empty();
    if (has_player) {
        session.add_robot(player_[0].robot, player_[0].x, player_[0].y);
    }
    for (size_t i = 0; i < enemies_.size(); i++) {
        if (has_player && enemies_[i].x == player_[0].x && enemies_[i].y == player_[0].y) {
            continue;
        }
        session.add_robot(enemies_[i].robot, enemies_[i].x, enemies_[i].y);
    }

    for (size_t i = 0; i < factory_.size(); i++) {
        session.add_fabric(factory_[i].factory, factory_[i].x, factory_[i].y);
    }

    return session;
}
