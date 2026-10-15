#ifndef OOP_PROJECT_SPRITE_CATALOG_H
#define OOP_PROJECT_SPRITE_CATALOG_H

#include "SDL.h"

#include <string>

#include "Canvas.h"
#include "../../game/includes/Game_session.h"

// Соответствие игровых объектов (роботов, статусов, предметов, способностей)
// файлам спрайтов из набора Tiny Swords. Пути — относительно папки ассетов.
class Sprite_catalog {
private:
	std::string get_status_icon(const std::string& status_name) const;
	std::string get_item_icon(const std::string& item_name) const;
	// область картинки предмета без пустых полей
	SDL_FRect get_item_icon_source(const std::string& item_name) const;

public:
	// спрайт зависит от стороны робота и (для врагов) от его способности
	std::string get_unit_sprite(const Game_session& session, int index) const;
	// вариант камня выбирается по координатам, чтобы препятствия не выглядели одинаково
	std::string get_rock_sprite(int x, int y) const;
	std::string get_grass_tileset() const;
	SDL_FRect get_grass_source() const;
	std::string get_factory_sprite() const;
	std::string get_ability_icon(const std::string& ability_name) const;
	// цвет статуса для подсветки и легенды
	SDL_Color get_status_color(const std::string& status_name) const;

	// offset — сдвиг фазы анимации (для горения)
	void draw_status_icon(Canvas& canvas, const std::string& status_name, int offset, const SDL_FRect& destination) const;
	void draw_item_icon(Canvas& canvas, const std::string& item_name, const SDL_FRect& destination) const;
};

#endif
