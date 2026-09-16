#include "Game_board.h"

#include <iostream>

using namespace std;

const size_t kMinSize = 8;
const size_t kMaxSize = 256;

GameBoard::GameBoard(size_t width, size_t height) {
    if (width < kMinSize) {
        cerr << "Error: width < " << kMinSize << ", set to " << kMinSize << "\n";
        width = kMinSize;
    }
    if (width > kMaxSize) {
        cerr << "Error: width > " << kMaxSize << ", set to " << kMaxSize << "\n";
        width = kMaxSize;
    }
    if (height < kMinSize) {
        cerr << "Error: height < " << kMinSize << ", set to " << kMinSize << "\n";
        height = kMinSize;
    }
    if (height > kMaxSize) {
        cerr << "Error: height > " << kMaxSize << ", set to " << kMaxSize << "\n";
        height = kMaxSize;
    }
    this->width = width;
    this->height = height;

    board.resize(height);
    // Убрать дублирование данных если не надо
    for (size_t y = 0; y < height; ++y) {
        board[y].resize(width);
        for (size_t x = 0; x < width; ++x) {
            board[y][x].x = (int) x;
            board[y][x].y = (int) y;
        }
    }
}

pair<size_t, size_t> GameBoard::get_board_size() const {
    return {width, height};
}

void GameBoard::set_square_speed_level(int x, int y, int level) {
    if (level < 1) {
        level = 1;
    }
    board[y][x].speed_level = level;
}

int GameBoard::get_square_speed_level(int x, int y) const {
    return board[y][x].speed_level;
}

void GameBoard::set_square_border(int x, int y, bool border) {
    board[y][x].border = border;
}

bool GameBoard::get_square_border(int x, int y) const {
    return board[y][x].border;
}

void GameBoard::set_square_visible(int x, int y, bool visible) {
    board[y][x].visible = visible;
}

bool GameBoard::get_square_visible(int x, int y) const {
    return board[y][x].visible;
}

void GameBoard::set_square_discovered(int x, int y, bool discovered) {
    board[y][x].discovered = discovered;
}

bool GameBoard::get_square_discovered(int x, int y) const {
    return board[y][x].discovered;
}

vector<pair<int, int>> GameBoard::get_square_cord() const {
    vector<pair<int, int>> result;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            result.push_back(make_pair(board[y][x].x, board[y][x].y));
        }
    }
    return result;
}


