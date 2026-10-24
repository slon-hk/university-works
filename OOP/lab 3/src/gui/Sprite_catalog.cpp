#include "includes/Sprite_catalog.h"

using namespace std;

const int kFireFrameMs = 200;
const float kFireFrameSize = 64.0f;

string Sprite_catalog::get_unit_sprite(const Game_session& session, int index) const {
	if (session.get_robots().is_player(index)) {
		return "Units/Blue Units/Warrior/Warrior_Idle.png";
	}
	if (session.get_robots().get_robot(index).get_relation() != Relation::Enemy) {
		return "Units/Yellow Units/Warrior/Warrior_Idle.png";
	}
	// враги отличаются внешним видом по своей способности
	if (session.get_robots().get_ability_count(index) > 0) {
		string ability = session.get_robots().get_ability(index, 0).get_name();
		if (ability == "Heal") {
			return "Units/Red Units/Monk/Idle.png";
		}
		if (ability == "Teleport") {
			return "Units/Red Units/Archer/Archer_Idle.png";
		}
	}
	return "Units/Red Units/Warrior/Warrior_Idle.png";
}

string Sprite_catalog::get_rock_sprite(int x, int y) const {
	// псевдослучайный, но стабильный выбор одного из 4 камней
	return "Terrain/Decorations/Rocks/Rock" + to_string((x * 7 + y * 13) % 4 + 1) + ".png";
}

string Sprite_catalog::get_grass_tileset() const {
	return "Terrain/Tileset/Tilemap_color1.png";
}

SDL_FRect Sprite_catalog::get_grass_source() const {
	// центральный тайл травы 64x64
	return { 64, 64, 64, 64 };
}

string Sprite_catalog::get_factory_sprite() const {
	return "Buildings/Red Buildings/Barracks.png";
}

string Sprite_catalog::get_ability_icon(const string& ability_name) const {
	if (ability_name == "Around attack") {
		return "UI Elements/UI Elements/Icons/Icon_05.png";
	}
	if (ability_name == "Long attack") {
		return "Units/Blue Units/Archer/Arrow.png";
	}
	if (ability_name == "Heal") {
		return "UI Elements/UI Elements/Icons/Icon_04.png";
	}
	return "UI Elements/UI Elements/Icons/Icon_07.png";
}

SDL_Color Sprite_catalog::get_status_color(const string& status_name) const {
	if (status_name == "Slow") {
		return { 80, 140, 255, 255 };
	}
	if (status_name == "Overload") {
		return { 190, 80, 255, 255 };
	}
	if (status_name == "Burn") {
		return { 255, 130, 30, 255 };
	}
	if (status_name == "Item") {
		return { 255, 220, 60, 255 };
	}
	return { 230, 230, 230, 255 };
}

string Sprite_catalog::get_status_icon(const string& status_name) const {
	if (status_name == "Slow") {
		return "UI Elements/UI Elements/Icons/Icon_08.png";
	}
	if (status_name == "Overload") {
		return "UI Elements/UI Elements/Icons/Icon_09.png";
	}
	if (status_name == "Burn") {
		return "Particle FX/Fire_03.png";
	}
	return "UI Elements/UI Elements/Icons/Icon_06.png";
}

string Sprite_catalog::get_item_icon(const string& item_name) const {
	if (item_name == "Health potion") {
		return "Terrain/Resources/Meat/Meat Resource/Meat Resource.png";
	}
	if (item_name == "Energy potion") {
		return "UI Elements/UI Elements/Icons/Icon_03.png";
	}
	return "Terrain/Resources/Tools/Tool_02.png";
}

SDL_FRect Sprite_catalog::get_item_icon_source(const string& item_name) const {
	if (item_name == "Health potion") {
		return { 9, 10, 47, 47 };
	}
	if (item_name == "Energy potion") {
		return { 4, 4, 55, 55 };
	}
	return { 18, 18, 27, 27 };
}

void Sprite_catalog::draw_status_icon(Canvas& canvas, const string& status_name, int offset, const SDL_FRect& destination) const {
	if (status_name == "Strength") {
		draw_item_icon(canvas, "Attack potion", destination);
		return;
	}
	if (status_name == "Item") {
		draw_item_icon(canvas, "Health potion", destination);
		return;
	}
	if (status_name == "Burn") {
		// огонь — анимация из ленты, используем кадры 2–4
		int frame = 2 + (canvas.get_animation_step(kFireFrameMs) + offset) % 3;
		SDL_FRect source = { frame * kFireFrameSize + 6, 16, 52, 48 };
		canvas.draw_image(get_status_icon(status_name), &source, destination);
		return;
	}
	canvas.draw_image(get_status_icon(status_name), nullptr, destination);
}

void Sprite_catalog::draw_item_icon(Canvas& canvas, const string& item_name, const SDL_FRect& destination) const {
	SDL_FRect source = get_item_icon_source(item_name);
	canvas.draw_image(get_item_icon(item_name), &source, destination);
}
