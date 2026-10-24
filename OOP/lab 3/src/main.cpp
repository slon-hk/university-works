#include "SDL.h"
#include "SDL_main.h"

#include "program/includes/Program.h"

int main(int argc, char* argv[]) {
	Program program(argc, argv);
	program.run();
	return 0;
}
