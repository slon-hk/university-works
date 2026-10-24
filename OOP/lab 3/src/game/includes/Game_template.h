#ifndef OOP_PROJECT_GAME_TEMPLATE_H
#define OOP_PROJECT_GAME_TEMPLATE_H

#include <memory>
#include <stdexcept>
#include <string>

#include "Command.h"
#include "Command_slots.h"
#include "Difficulty.h"
#include "Display_mode_selector.h"
#include "Game.h"
#include "Game_session.h"
#include "Player_controller.h"
#include "Player_state.h"

// значение Level_count для бесконечного режима
const int kEndless_levels = 0;

// Игра из нескольких уровней: инициализация, переход между уровнями
// и завершение игры победой или поражением. Ввода, отрисовки и работы
// с файлами здесь нет — игра получает готовые команды через handle()
// и отдаёт состояние; сохранение и загрузка выполняются снаружи.
//
// Level_generator — класс, создающий уровни. Должен иметь
//   std::string get_name() const;
//   Game_session create(int level_number, const Difficulty& difficulty) const; // уровни с 1
// Level_count — количество уровней или kEndless_levels: тогда уровни
// не кончаются и игра идёт до поражения.
//
// При переходе на следующий уровень игрок сохраняет ранг, опыт, здоровье,
// способности с улучшениями и инвентарь; энергия восстанавливается, статусы
// снимаются (кроме модификатора сложности Statuses_persist). Сложность
// задаётся при старте и действует на все уровни.
template <typename Level_generator, int Level_count>
class Game_template : public Game {
	static_assert(Level_count >= 0, "Level_count must be positive or kEndless_levels");

private:
	Level_generator generator_;
	Command_slots slots_;
	Difficulty difficulty_;
	int level_;
	// контроллер ссылается на сессию, поэтому оба пересоздаются на каждом уровне
	std::unique_ptr<Game_session> session_;
	std::unique_ptr<Player_controller> controller_;
	Display_mode_selector display_mode_;
	Game_phase phase_;
	Game_event_listener* listener_;

	bool is_endless() const {
		return Level_count == kEndless_levels;
	}

	std::string get_level_title() const {
		std::string title = "=== Level " + std::to_string(level_);
		if (!is_endless()) {
			title += " / " + std::to_string(Level_count);
		}
		return title + " ===";
	}

	void start_level(const Player_state& player) {
		session_ = std::make_unique<Game_session>(generator_.create(level_, difficulty_));
		session_->set_event_listener(listener_);
		session_->set_difficulty(difficulty_);
		session_->set_player_state(player);
		session_->log_event(get_level_title());
		session_->log_event("Difficulty: " + difficulty_.get_name());
		for (Modifier modifier : difficulty_.get_modifiers()) {
			session_->log_event("  " + difficulty_.get_modifier_description(modifier));
		}
		controller_ = std::make_unique<Player_controller>(*session_);
		// на первом уровне у игрока ещё нет способностей
		bool has_abilities = session_->get_robots().get_ability_count(Game_session::kPlayerIndex) > 0;
		phase_ = has_abilities ? Game_phase::Playing : Game_phase::Start_ability;
	}

	void start_next_level() {
		Player_state player = session_->get_player_state();
		player.prepare_for_next_level(difficulty_.has_modifier(Modifier::Statuses_persist));
		if (!is_endless() && level_ >= Level_count) {
			session_->log_event("All " + std::to_string(Level_count) + " levels cleared: victory");
			phase_ = Game_phase::Victory;
			return;
		}
		session_->log_event("Moving to level " + std::to_string(level_ + 1) + " with rank "
							+ std::to_string(player.get_robot().get_rank()) + ", HP "
							+ std::to_string(player.get_robot().get_current_health()));
		level_++;
		start_level(player);
	}

	void handle_start_ability(Command command) {
		if (!slots_.is_ability(command)) {
			return;
		}
		controller_->choose_start_ability(slots_.get_ability_slot(command));
		if (session_->get_robots().get_ability_count(Game_session::kPlayerIndex) > 0) {
			phase_ = Game_phase::Playing;
		}
	}

	void handle_playing(Command command) {
		bool is_ability = slots_.is_ability(command);
		// пока не выбрана награда за ранг, принимаем только выбор награды
		if (session_->has_rank_reward()) {
			if (is_ability) {
				controller_->choose_rank_reward(slots_.get_ability_slot(command));
			}
		} else if (command == Command::Move_up) {
			controller_->move(Direction::UP);
		} else if (command == Command::Move_down) {
			controller_->move(Direction::DOWN);
		} else if (command == Command::Move_left) {
			controller_->move(Direction::LEFT);
		} else if (command == Command::Move_right) {
			controller_->move(Direction::RIGHT);
		} else if (command == Command::Use_ability) {
			controller_->activate_selected_ability();
		} else if (command == Command::Cancel) {
			controller_->cancel_targeting();
		} else if (command == Command::End_turn) {
			controller_->end_turn();
		} else if (is_ability) {
			controller_->select_ability(slots_.get_ability_slot(command));
		} else if (slots_.is_item(command)) {
			controller_->use_item(slots_.get_item_slot(command));
		}

		if (session_->is_game_over()) {
			phase_ = Game_phase::Game_over;
		} else if (session_->is_level_complete() && !session_->has_rank_reward()) {
			phase_ = Game_phase::Level_complete;
		}
	}

public:
	Game_template() : level_(1), phase_(Game_phase::Finished), listener_(nullptr) {
	}

	// новая игра с первого уровня и стартовым роботом
	void start_new(const Difficulty& difficulty) override {
		difficulty_ = difficulty;
		level_ = 1;
		start_level(Player_state());
	}

	// Бросает runtime_error, если не удалось создать следующий уровень.
	void handle(Command command) override {
		if (command == Command::Quit || phase_ == Game_phase::Finished) {
			phase_ = Game_phase::Finished;
			return;
		}
		if (command == Command::Switch_display) {
			display_mode_.next();
			return;
		}
		// выйти в меню можно в любой момент; прогресс сохранён на начало уровня
		if (command == Command::Menu) {
			phase_ = Game_phase::Finished;
			return;
		}

		if (phase_ == Game_phase::Start_ability) {
			handle_start_ability(command);
		} else if (phase_ == Game_phase::Playing) {
			handle_playing(command);
		} else if (phase_ == Game_phase::Level_complete) {
			start_next_level();
		} else {
			// Game_over и Victory: любая команда завершает игру
			phase_ = Game_phase::Finished;
		}
	}

	Game_phase get_phase() const override {
		return phase_;
	}

	int get_level() const override {
		return level_;
	}

	int get_level_count() const override {
		return Level_count;
	}

	const Game_session& get_session() const override {
		if (session_ == nullptr) {
			throw std::logic_error("Game has not been started");
		}
		return *session_;
	}

	const Player_controller& get_controller() const override {
		if (controller_ == nullptr) {
			throw std::logic_error("Game has not been started");
		}
		return *controller_;
	}

	const Display_mode_selector& get_display_mode() const override {
		return display_mode_;
	}

	void set_event_listener(Game_event_listener* listener) override {
		listener_ = listener;
		if (session_ != nullptr) {
			session_->set_event_listener(listener);
		}
	}

	// например "random_5" или "random_endless"
	std::string get_mode_name() const override {
		std::string count = is_endless() ? "endless" : std::to_string(Level_count);
		return generator_.get_name() + "_" + count;
	}

	bool can_act() const override {
		return phase_ == Game_phase::Playing && !session_->has_rank_reward();
	}

	void restore(int level, std::unique_ptr<Game_session> session, int selected_slot,
				 bool ability_switched) override {
		if (level < 1 || (!is_endless() && level > Level_count)) {
			throw std::runtime_error("level " + std::to_string(level) + " is out of range for mode " + get_mode_name());
		}
		if (session == nullptr || session->get_robots().get_count() == 0 || session->is_game_over()
			|| session->is_level_complete() || session->has_rank_reward()) {
			throw std::runtime_error("player cannot act in the saved state");
		}
		const Ability_catalog catalog;
		if (selected_slot < 0 || selected_slot >= catalog.get_count()
			|| session->get_robots().find_ability(Game_session::kPlayerIndex, catalog.get_name(selected_slot)) < 0) {
			throw std::runtime_error("selected ability is not learned");
		}
		// все проверки пройдены — только теперь меняем состояние
		std::unique_ptr<Player_controller> controller = std::make_unique<Player_controller>(*session);
		controller->restore(selected_slot, ability_switched);
		level_ = level;
		difficulty_ = session->get_difficulty();
		session->set_event_listener(listener_);
		session_ = std::move(session);
		controller_ = std::move(controller);
		phase_ = Game_phase::Playing;
	}
};

#endif
