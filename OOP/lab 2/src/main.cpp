#include "SDL.h"
#include "SDL_main.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "game/includes/Demo_level.h"
#include "game/includes/Game_session.h"
#include "gui/includes/Gui.h"

using namespace std;

int main(int argc, char* argv[]) {
	srand((unsigned) time(nullptr));

	Demo_level level;
	Game_session session = level.create();
	Gui gui(session);
	Level_result result = gui.run();
	cout << "result: " << (int) result << endl;   // 0 Completed, 1 Lost, 2 Exit_to_menu, 3 Quit
	return 0;
	return 0;
}
