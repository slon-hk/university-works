#ifndef OOP_PROJECT_GAME_BOARD_H
#define OOP_PROJECT_GAME_BOARD_H

#include <cstddef>
#include <utility>
#include <vector>


class GameBoard {
private:
    struct Square {
        int x = 0;
        int y = 0;
        int speed_level = 1;
        bool border = false;
        bool visible = false;
        bool discovered = false;
        // парметр изученности 
    };

    std::size_t width = 32;
    std::size_t height = 32;
    std::vector<std::vector<Square>> board;

public:
    GameBoard(std::size_t width, std::size_t height);

    std::pair<std::size_t, std::size_t> get_board_size() const;

    std::vector<std::pair<int, int>> get_square_cord() const;

    void set_square_speed_level(int x, int y, int level);
    int get_square_speed_level(int x, int y) const;

    void set_square_border(int x, int y, bool border);
    bool get_square_border(int x, int y) const;

    void set_square_visible(int x, int y, bool visible);
    bool get_square_visible(int x, int y) const;

    void set_square_discovered(int x, int y, bool discovered);
    bool get_square_discovered(int x, int y) const;
};

#endif
