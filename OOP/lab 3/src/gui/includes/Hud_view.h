#ifndef OOP_PROJECT_HUD_VIEW_H
#define OOP_PROJECT_HUD_VIEW_H

#include <string>

#include "Canvas.h"
#include "../../game/includes/Command.h"
#include "../../game/includes/Key_labels.h"
#include "../../game/includes/Display_mode_selector.h"
#include "../../game/includes/Game_session.h"
#include "../../game/includes/Player_controller.h"
#include "Layout.h"
#include "Sprite_catalog.h"

// Отрисовка интерфейса вокруг поля: характеристики игрока, инвентарь
// и журнал в боковой панели, слоты способностей и подсказки внизу.
class Hud_view {
private:
	Canvas& canvas_;
	const Layout& layout_;
	const Sprite_catalog& sprites_;
	const Key_labels& labels_;

	void draw_player_stats(const Game_session& session, const Display_mode_selector& display_mode);
	void draw_inventory(const Game_session& session);
	void draw_log(const Game_session& session);
	// оповещение о сохранении/загрузке или подсказка по клавишам
	void draw_notification(const std::string& notification);
	void draw_ability_slot(const Game_session& session, const Player_controller& controller, int slot);
	void draw_hints(const Game_session& session, const Player_controller& controller);
	// легенда иконок статусов
	void draw_legend(float x, float y);

public:
	Hud_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites, const Key_labels& labels);

	void draw(const Game_session& session, const Player_controller& controller,
			  const Display_mode_selector& display_mode, const std::string& notification);
};

#endif
