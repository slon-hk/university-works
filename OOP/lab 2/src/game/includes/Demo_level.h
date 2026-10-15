#ifndef OOP_PROJECT_DEMO_LEVEL_H
#define OOP_PROJECT_DEMO_LEVEL_H

#include "Game_session.h"

// Демонстрационный уровень: расставляет игрока, врагов, союзника,
// препятствия, «вязкие» клетки и завод с помощью Map_editor.
class Demo_level {
public:
	Game_session create() const;
};

#endif
