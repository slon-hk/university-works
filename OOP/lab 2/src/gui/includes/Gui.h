#ifndef OOP_PROJECT_GUI_H
#define OOP_PROJECT_GUI_H

#include "SDL.h"

#include "Board_view.h"
#include "Canvas.h"
#include "Dialog_view.h"
#include "Display_mode_selector.h"
#include "../../game/includes/Game_session.h"
#include "Hud_view.h"
#include "Layout.h"
#include "Player_controller.h"
#include "Sprite_catalog.h"
#include "../../game/includes/Level_result.h"

// Главное окно игры на SDL3: создаёт окно и представления, крутит цикл
// обработки событий и отрисовки, распределяет ввод между контроллером
// и переключателем режима отображения.
class Gui {
private:
	Game_session& session_;
	Layout layout_;
	Sprite_catalog sprites_;
	Player_controller controller_;
	Display_mode_selector display_mode_;
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	Canvas* canvas_;
	Board_view* board_view_;
	Hud_view* hud_view_;
	Dialog_view* dialog_view_;

	// экран выбора стартовой способности; false, если окно закрыли
	bool run_start_screen();
	void handle_key(SDL_Scancode key);
	// клик по слоту инвентаря или по целевой клетке
	void handle_click(float x, float y);
	void render();
	// ждёт нажатия клавиши, продолжая рисовать кадр; false, если окно закрыли
	bool wait_for_key();

public:
	explicit Gui(Game_session& session);
	~Gui();

	// основной цикл игры до закрытия окна
	Level_result run();
};

#endif
