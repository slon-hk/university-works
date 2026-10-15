#ifndef OOP_PROJECT_CANVAS_H
#define OOP_PROJECT_CANVAS_H

#include "SDL.h"
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "Assets.h"

// Обёртка над SDL_Renderer: примитивы рисования, текст двумя шрифтами
// (заголовки и обычный текст), картинки и покадровая анимация спрайтов.
class Canvas {
private:
	SDL_Renderer* renderer_;
	TTF_Font* title_font_;
	TTF_Font* text_font_;
	Assets assets_;

	void draw_text_with_font(TTF_Font* font, const std::string& text, float x, float y, SDL_Color color);

public:
	Canvas(SDL_Renderer* renderer, const std::string& font_path, const std::string& assets_root);
	~Canvas();


	void clear(SDL_Color color);
	void present();
	void fill_rect(const SDL_FRect& rect, SDL_Color color);
	void draw_rect(const SDL_FRect& rect, SDL_Color color, int thickness = 1);

	// текст обычным и крупным шрифтом; (x, y) — левый верхний угол
	void draw_text(const std::string& text, float x, float y, SDL_Color color);
	void draw_title(const std::string& text, float x, float y, SDL_Color color);

	// рисует картинку (или её фрагмент source) с умножением цвета на tint
	void draw_image(const std::string& path, const SDL_FRect* source, const SDL_FRect& destination,
					SDL_Color tint = { 255, 255, 255, 255 });
	// рисует кадр frame из ленты анимации; flip — отражение по горизонтали
	void draw_frame(const std::string& path, int frame, const SDL_FRect& destination, bool flip);
	// рисует текущий кадр анимации; offset сдвигает фазу, чтобы
	// одинаковые спрайты не анимировались синхронно
	void draw_animated(const std::string& path, int offset, const SDL_FRect& destination, bool flip);
	// номер шага анимации по текущему времени при длительности кадра frame_ms
	int get_animation_step(int frame_ms) const;
};

#endif
