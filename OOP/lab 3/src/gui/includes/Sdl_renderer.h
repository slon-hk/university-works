#ifndef OOP_PROJECT_SDL_RENDERER_H
#define OOP_PROJECT_SDL_RENDERER_H

#include "SDL.h"

#include <string>

#include "../../game/includes/Command.h"
#include "../../game/includes/Key_labels.h"
#include "../../game/includes/Game.h"
#include "Board_view.h"
#include "Canvas.h"
#include "Dialog_view.h"
#include "Hud_view.h"
#include "Layout.h"
#include "Sprite_catalog.h"

// Визуализация на SDL3: окно и представления. Только рисует переданное
// состояние, ввод не считывает. labels — подписи клавиш для подсказок.
class Sdl_renderer {
private:
	Layout layout_;
	Sprite_catalog sprites_;
	Key_labels labels_;
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	Canvas* canvas_;
	Board_view* board_view_;
	Hud_view* hud_view_;
	Dialog_view* dialog_view_;

	// подгоняет окно под размер поля уровня
	void fit_window(const Game_session& session);

public:
	explicit Sdl_renderer(const Key_labels& labels);
	~Sdl_renderer();

	Sdl_renderer(const Sdl_renderer&) = delete;
	Sdl_renderer& operator=(const Sdl_renderer&) = delete;

	// каждый метод рисует один кадр целиком
	void draw_menu(bool has_save);
	void draw_difficulty_menu(int mode);
	void draw_message(const std::string& title, const std::string& text);
	// notification — строка-оповещение (результат сохранения/загрузки)
	void draw_game(const Game& game, const std::string& notification);
};

#endif
