#ifndef OOP_PROJECT_DIALOG_VIEW_H
#define OOP_PROJECT_DIALOG_VIEW_H

#include <string>

#include "../../abilities/includes/Ability_catalog.h"
#include "../../game/includes/Command.h"
#include "../../game/includes/Key_labels.h"
#include "../../game/includes/Difficulty.h"
#include "../../game/includes/Game_mode_catalog.h"
#include "Canvas.h"
#include "../../game/includes/Game_session.h"
#include "Layout.h"
#include "Sprite_catalog.h"

// Отрисовка экранов и диалогов: главное меню, сообщения, выбор стартовой
// способности, награда за ранг и экраны конца уровня.
class Dialog_view {
private:
	Canvas& canvas_;
	const Layout& layout_;
	const Sprite_catalog& sprites_;
	const Key_labels& labels_;
	Game_mode_catalog modes_;


public:
	Dialog_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites, const Key_labels& labels);

	// рисуют экран целиком, включая clear/present
	void draw_menu(bool has_save);
	// mode — номер режима в Game_mode_catalog
	void draw_difficulty_menu(int mode);
	void draw_message(const std::string& title, const std::string& text);
	void draw_start_screen(const Ability_catalog& catalog, const Difficulty& difficulty);
	// окна поверх поля (кадр завершает вызывающий код)
	void draw_rank_reward(const Game_session& session, const Ability_catalog& catalog);
	void draw_game_over(const Game_session& session, int level);
	void draw_level_complete(const Game_session& session, int level);
};

#endif
