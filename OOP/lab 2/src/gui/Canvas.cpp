#include "includes/Canvas.h"

#include <iostream>

using namespace std;

const float kTitleFontSize = 24.0f;
const float kTextFontSize = 16.0f;
const int kAnimationFrameMs = 100;

Canvas::Canvas(SDL_Renderer* renderer, const string& font_path, const string& assets_root)
	: renderer_(renderer), title_font_(nullptr), text_font_(nullptr), assets_(renderer, assets_root) {
	TTF_Init();
	title_font_ = TTF_OpenFont(font_path.c_str(), kTitleFontSize);
	text_font_ = TTF_OpenFont(font_path.c_str(), kTextFontSize);
	if (title_font_ == nullptr || text_font_ == nullptr) {
		cout << "Не удалось загрузить шрифт: " << SDL_GetError() << endl;
	}
}

Canvas::~Canvas() {
	if (title_font_ != nullptr) {
		TTF_CloseFont(title_font_);
	}
	if (text_font_ != nullptr) {
		TTF_CloseFont(text_font_);
	}
	TTF_Quit();
}

void Canvas::clear(SDL_Color color) {
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer_);
}

void Canvas::present() {
	SDL_RenderPresent(renderer_);
}

void Canvas::fill_rect(const SDL_FRect& rect, SDL_Color color) {
	// смешивание включаем только для полупрозрачных цветов
	SDL_SetRenderDrawBlendMode(renderer_, color.a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer_, &rect);
	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
}

void Canvas::draw_rect(const SDL_FRect& rect, SDL_Color color, int thickness) {
	SDL_SetRenderDrawBlendMode(renderer_, color.a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	// толстую рамку рисуем несколькими вложенными прямоугольниками
	for (int i = 0; i < thickness; i++) {
		SDL_FRect frame = { rect.x + i, rect.y + i, rect.w - 2 * i, rect.h - 2 * i };
		SDL_RenderRect(renderer_, &frame);
	}
	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
}

void Canvas::draw_text(const string& text, float x, float y, SDL_Color color) {
	draw_text_with_font(text_font_, text, x, y, color);
}

void Canvas::draw_title(const string& text, float x, float y, SDL_Color color) {
	draw_text_with_font(title_font_, text, x, y, color);
}

void Canvas::draw_text_with_font(TTF_Font* font, const string& text, float x, float y, SDL_Color color) {
	if (font == nullptr || text.empty()) {
		return;
	}
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
	if (surface == nullptr) {
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_FRect destination = { x, y, (float) surface->w, (float) surface->h };
	SDL_RenderTexture(renderer_, texture, nullptr, &destination);
	SDL_DestroyTexture(texture);
	SDL_DestroySurface(surface);
}

void Canvas::draw_image(const string& path, const SDL_FRect* source, const SDL_FRect& destination, SDL_Color tint) {
	SDL_Texture* texture = assets_.get(path);
	if (texture == nullptr) {
		return;
	}
	SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
	SDL_SetTextureAlphaMod(texture, tint.a);
	SDL_RenderTexture(renderer_, texture, source, &destination);
	// текстура общая (из кеша), поэтому возвращаем модификаторы по умолчанию
	SDL_SetTextureColorMod(texture, 255, 255, 255);
	SDL_SetTextureAlphaMod(texture, 255);
}

void Canvas::draw_frame(const string& path, int frame, const SDL_FRect& destination, bool flip) {
	SDL_Texture* texture = assets_.get(path);
	if (texture == nullptr) {
		return;
	}
	float width = 0;
	float height = 0;
	SDL_GetTextureSize(texture, &width, &height);
	int count = assets_.get_frame_count(texture);
	SDL_FRect source = { (float) (frame % count) * height, 0, height, height };
	SDL_RenderTextureRotated(renderer_, texture, &source, &destination, 0, nullptr,
							 flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Canvas::draw_animated(const string& path, int offset, const SDL_FRect& destination, bool flip) {
	draw_frame(path, get_animation_step(kAnimationFrameMs) + offset, destination, flip);
}

int Canvas::get_animation_step(int frame_ms) const {
	return (int) (SDL_GetTicks() / (Uint64) frame_ms);
}
