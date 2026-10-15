#ifndef OOP_PROJECT_DIALOG_VIEW_H
#define OOP_PROJECT_DIALOG_VIEW_H

#include "../../abilities/includes/Ability_catalog.h"
#include "Canvas.h"
#include "../../game/includes/Game_session.h"
#include "Layout.h"
#include "Sprite_catalog.h"

// Отрисовка экранов и диалогов: выбор стартовой способности,
// награда за ранг и экран поражения.
class Dialog_view {
private:
	Canvas& canvas_;
	const Layout& layout_;
	const Sprite_catalog& sprites_;

public:
	Dialog_view(Canvas& canvas, const Layout& layout, const Sprite_catalog& sprites);

	// рисует экран целиком, включая clear/present
	void draw_start_screen(const Ability_catalog& catalog);
	// окна поверх поля (кадр завершает вызывающий код)
	void draw_rank_reward(const Game_session& session, const Ability_catalog& catalog);
	void draw_game_over(const Game_session& session);
	void draw_level_complete(const Game_session& session);
};

#endif
