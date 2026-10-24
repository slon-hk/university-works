#include "includes/Application.h"

#include <stdexcept>

using namespace std;

Application::Application(const string& save_path, Logger& logger)
	: save_slot_(save_path), logger_(logger), game_listener_(logger), screen_(Screen::Menu), running_(true),
	  pending_mode_(0) {
}

void Application::handle(Command command) {
	if (command == Command::Quit) {
		logger_.log(Log_level::Program_info, "Window closed, quitting"
					+ (screen_ == Screen::Game ? " (unsaved progress of " + describe_game() + " is lost)" : string()));
		running_ = false;
		return;
	}
	if (screen_ == Screen::Message) {
		screen_ = Screen::Menu;
	} else if (screen_ == Screen::Menu) {
		handle_menu(command);
	} else if (screen_ == Screen::Difficulty_select) {
		handle_difficulty(command);
	} else {
		handle_game(command);
	}
}

void Application::handle_menu(Command command) {
	if (command == Command::Cancel) {
		logger_.log(Log_level::Program_info, "Quit from the main menu");
		running_ = false;
		return;
	}
	int slot = slots_.is_ability(command) ? slots_.get_ability_slot(command) : -1;
	if (slot >= 0 && slot < modes_.get_count()) {
		pending_mode_ = slot;
		screen_ = Screen::Difficulty_select;
		logger_.log(Log_level::Program_info, "Mode selected: " + modes_.get_title(slot));
		return;
	}
	// пункт загрузки идёт сразу после режимов
	if (command != Command::Load && slot != modes_.get_count()) {
		return;
	}
	logger_.log(Log_level::Program_info, "Load attempt from the main menu: " + save_slot_.get_path());
	if (!has_save()) {
		logger_.log(Log_level::Program_error, "Load failed: no save file " + save_slot_.get_path());
		return;
	}
	string error;
	if (!load_game(error)) {
		// ошибка в файле сохранения — сообщаем и остаёмся в меню
		show_message("Error", error);
	}
}

void Application::handle_difficulty(Command command) {
	if (command == Command::Cancel) {
		logger_.log(Log_level::Program_info, "Difficulty selection cancelled, back to the main menu");
		screen_ = Screen::Menu;
		return;
	}
	if (!slots_.is_ability(command) || slots_.get_ability_slot(command) >= kDifficultyLevelCount) {
		return;
	}
	Difficulty difficulty((Difficulty_level) slots_.get_ability_slot(command));
	unique_ptr<Game> game;
	try {
		game = modes_.create_new(pending_mode_, difficulty, &game_listener_);
	} catch (const exception& error) {
		// например, нет файла фиксированного уровня
		logger_.log(Log_level::Program_error, string("Cannot start a new game: ") + error.what());
		show_message("Error", error.what());
		return;
	}
	enter_game(std::move(game), "New game started");
}

void Application::handle_game(Command command) {
	notification_.clear();
	if (command == Command::Save) {
		save_game();
		return;
	}
	if (command == Command::Load) {
		load_in_game();
		return;
	}

	Game_phase phase_before = game_->get_phase();
	try {
		game_->handle(command);
	} catch (const exception& error) {
		// например, не удалось прочитать файл следующего уровня
		logger_.log(Log_level::Program_error, string("Game stopped: ") + error.what());
		game_.reset();
		show_message("Error", error.what());
		return;
	}
	if (game_->get_phase() != Game_phase::Finished) {
		if (game_->get_phase() == Game_phase::Game_over && phase_before != Game_phase::Game_over) {
			logger_.log(Log_level::Program_info, "Game over: " + describe_game());
		} else if (game_->get_phase() == Game_phase::Victory && phase_before != Game_phase::Victory) {
			logger_.log(Log_level::Program_info, "Victory: " + describe_game());
		}
		return;
	}
	if (command == Command::Menu) {
		logger_.log(Log_level::Program_info, "Exit to the main menu from " + describe_game());
	} else {
		logger_.log(Log_level::Program_info, "Game finished, back to the main menu");
	}
	game_.reset();
	screen_ = Screen::Menu;
}

void Application::enter_game(unique_ptr<Game> game, const string& event) {
	game_ = std::move(game);
	notification_.clear();
	screen_ = Screen::Game;
	logger_.log(Log_level::Program_info, event + ": " + describe_game());
}

void Application::save_game() {
	logger_.log(Log_level::Program_info, "Save attempt: " + save_slot_.get_path());
	if (!game_->can_act()) {
		notification_ = "Cannot save now";
		logger_.log(Log_level::Program_error, "Save refused: the player cannot act now");
		return;
	}
	try {
		save_slot_.save(*game_);
		notification_ = "Game saved";
		logger_.log(Log_level::Program_info, "Game saved: " + describe_game());
	} catch (const exception& error) {
		notification_ = string("Save failed: ") + error.what();
		logger_.log(Log_level::Program_error, notification_);
	}
}

void Application::load_in_game() {
	logger_.log(Log_level::Program_info, "Load attempt during the game: " + save_slot_.get_path());
	if (!game_->can_act()) {
		notification_ = "Cannot load now";
		logger_.log(Log_level::Program_error, "Load refused: the player cannot act now");
		return;
	}
	string error;
	notification_ = load_game(error) ? "Game loaded" : "Load failed: " + error;
}

bool Application::load_game(string& error) {
	unique_ptr<Game> game;
	try {
		game = save_slot_.load(&game_listener_);
	} catch (const exception& load_error) {
		error = load_error.what();
		// текущая игра (если есть) заменяется только после успешной загрузки
		logger_.log(Log_level::Program_error, "Load failed: " + error
					+ (game_ != nullptr ? "; current game is kept" : string()));
		return false;
	}
	enter_game(std::move(game), "Game loaded");
	return true;
}

string Application::describe_game() const {
	if (game_ == nullptr) {
		return "no game";
	}
	const Difficulty& difficulty = game_->get_session().get_difficulty();
	string text = "mode " + game_->get_mode_name() + ", level " + to_string(game_->get_level()) + ", turn "
		+ to_string(game_->get_session().get_turn()) + ", difficulty " + difficulty.get_name();
	for (Modifier modifier : difficulty.get_modifiers()) {
		text += "; " + difficulty.get_modifier_description(modifier);
	}
	return text;
}

void Application::show_message(const string& title, const string& text) {
	message_title_ = title;
	message_text_ = text;
	screen_ = Screen::Message;
}

bool Application::is_running() const {
	return running_;
}

Screen Application::get_screen() const {
	return screen_;
}

bool Application::has_save() const {
	return save_slot_.exists();
}

int Application::get_pending_mode() const {
	return pending_mode_;
}

const string& Application::get_message_title() const {
	return message_title_;
}

const string& Application::get_message_text() const {
	return message_text_;
}

const string& Application::get_notification() const {
	return notification_;
}

const Game& Application::get_game() const {
	if (screen_ != Screen::Game || game_ == nullptr) {
		throw logic_error("No game is running");
	}
	return *game_;
}
