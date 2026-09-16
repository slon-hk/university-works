#ifndef OOP_PROJECT_GUI_H
#define OOP_PROJECT_GUI_H

#include "SDL.h"
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "GameSession.h"

class Gui {
private:
    GameSession& session_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    TTF_Font* font_;

    void draw_text(const std::string& text, float x, float y);
    void draw_board();
    void draw_factories();
    void draw_robots();
    void draw_stats();
    void render();

public:
    Gui(GameSession& session);
    ~Gui();

    void run();
};

#endif