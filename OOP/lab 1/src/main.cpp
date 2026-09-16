#include "SDL.h"
#include "SDL_main.h"

#include <cstdlib>
#include <ctime>

#include "Robot.h"
#include "Robot_factory.h"
#include "MapEditor.h"
#include "GameSession.h"
#include "GUI.h"

int main(int argc, char* argv[]) {
    srand((unsigned) time(nullptr));

    MapEditor editor(32, 32);

    Robot player(100, 100, 10, 10, 10, 0, 1, 1, -1, 10, 0);
    editor.set_player(player, 10, 10);

    Robot enemy(100, 100, 10, 10, 10, 0, 100, 1, 5, 10, -1);
    editor.add_enemy(enemy, 11, 10);

    Robot enemy_1(100, 100, 10, 10, 10, 0, 100, 1, 5, 10, 1);
    editor.add_enemy(enemy_1, 11, 20);

    editor.add_obstacle(15, 13);
    editor.add_obstacle(15, 14);
    editor.add_obstacle(15, 15);
    editor.add_obstacle(15, 16);
    editor.add_obstacle(15, 17);

    editor.add_speed_level(30, 20, 10);
    editor.add_speed_level(31, 20, 10);
    editor.add_speed_level(32, 20, 11);

    Robot factory_unit(100, 100, 10, 10, 10, 0, 100, 1, 5, 10, -1);
    Robot_factory factory(factory_unit, 1, 2);
    editor.add_factory(factory, 20, 15);

    GameSession session = editor.build();
    Gui gui(session);
    gui.run();

    return 0;
}