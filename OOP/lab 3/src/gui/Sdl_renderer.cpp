#include "includes/Sdl_renderer.h"
#include "includes/Palette.h"

using namespace std;

// размер поля, под который открывается окно до первого уровня
const int kDefaultBoardSize = 32;

Sdl_renderer::Sdl_renderer(const Key_labels& labels)
	: layout_(kDefaultBoardSize, kDefaultBoardSize), labels_(labels), window_(nullptr), renderer_(nullptr),
	  canvas_(nullptr), board_view_(nullptr), hud_view_(nullptr), dialog_view_(nullptr) {
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("Game", layout_.get_window_width(), layout_.get_window_height(), 0,
								&window_, &renderer_);
	// без вертикальной синхронизации кадры выводятся без ограничения частоты и текст дрожит
	SDL_SetRenderVSync(renderer_, 1);
	// FONT_PATH и ASSETS_PATH задаются в CMakeLists.txt
	canvas_ = new Canvas(renderer_, FONT_PATH, string(ASSETS_PATH) + "/Tiny Swords (Free Pack)");
	board_view_ = new Board_view(*canvas_, layout_, sprites_);
	hud_view_ = new Hud_view(*canvas_, layout_, sprites_, labels_);
	dialog_view_ = new Dialog_view(*canvas_, layout_, sprites_, labels_);
}

Sdl_renderer::~Sdl_renderer() {
	delete dialog_view_;
	delete hud_view_;
	delete board_view_;
	delete canvas_;
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Sdl_renderer::fit_window(const Game_session& session) {
	pair<size_t, size_t> size = session.get_board().get_board_size();
	Layout layout((int) size.first, (int) size.second);
	if (layout.get_window_width() != layout_.get_window_width()
		|| layout.get_window_height() != layout_.get_window_height()) {
		// представления хранят ссылку на layout_, поэтому присваиваем, а не пересоздаём
		layout_ = layout;
		SDL_SetWindowSize(window_, layout_.get_window_width(), layout_.get_window_height());
	}
}

void Sdl_renderer::draw_menu(bool has_save) {
	dialog_view_->draw_menu(has_save);
}

void Sdl_renderer::draw_difficulty_menu(int mode) {
	dialog_view_->draw_difficulty_menu(mode);
}

void Sdl_renderer::draw_message(const string& title, const string& text) {
	dialog_view_->draw_message(title, text);
}

void Sdl_renderer::draw_game(const Game& game, const string& notification) {
	Game_phase phase = game.get_phase();
	if (phase == Game_phase::Victory) {
		dialog_view_->draw_message("VICTORY", "All " + to_string(game.get_level_count()) + " levels cleared!");
		return;
	}
	if (phase == Game_phase::Start_ability) {
		dialog_view_->draw_start_screen(game.get_controller().get_catalog(), game.get_session().get_difficulty());
		return;
	}

	const Game_session& session = game.get_session();
	fit_window(session);
	canvas_->clear(Palette::kBackground);
	board_view_->draw(session, game.get_controller(), game.get_display_mode());
	hud_view_->draw(session, game.get_controller(), game.get_display_mode(), notification);
	if (session.has_rank_reward()) {
		dialog_view_->draw_rank_reward(session, game.get_controller().get_catalog());
	}
	if (phase == Game_phase::Level_complete) {
		dialog_view_->draw_level_complete(session, game.get_level());
	}
	if (phase == Game_phase::Game_over) {
		dialog_view_->draw_game_over(session, game.get_level());
	}
	canvas_->present();
}
