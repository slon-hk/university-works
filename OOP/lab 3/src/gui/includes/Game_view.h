#ifndef OOP_PROJECT_GAME_VIEW_H
#define OOP_PROJECT_GAME_VIEW_H

#include "../../game/includes/Application.h"

// Связывает игру с системой визуализации: по текущему экрану приложения
// решает, что показать, и передаёт состояние в Renderer, ничего не зная
// о том, как именно оно рисуется. Renderer должен иметь методы
//   void draw_menu(bool has_save);
//   void draw_difficulty_menu(int mode); // mode: 0 — фиксированные, 1 — случайные, 2 — бесконечный
//   void draw_message(const std::string& title, const std::string& text);
//   void draw_game(const Game& game, const std::string& notification);
template <typename Renderer>
class Game_view {
private:
	Renderer& renderer_;
	const Application& application_;

public:
	Game_view(Renderer& renderer, const Application& application)
		: renderer_(renderer), application_(application) {
	}

	// рисует один кадр
	void draw() {
		Screen screen = application_.get_screen();
		if (screen == Screen::Menu) {
			renderer_.draw_menu(application_.has_save());
		} else if (screen == Screen::Difficulty_select) {
			renderer_.draw_difficulty_menu(application_.get_pending_mode());
		} else if (screen == Screen::Message) {
			renderer_.draw_message(application_.get_message_title(), application_.get_message_text());
		} else {
			renderer_.draw_game(application_.get_game(), application_.get_notification());
		}
	}
};

#endif
