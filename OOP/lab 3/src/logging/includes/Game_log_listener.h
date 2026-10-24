#ifndef OOP_PROJECT_GAME_LOG_LISTENER_H
#define OOP_PROJECT_GAME_LOG_LISTENER_H

#include "../../game/includes/Game_event_listener.h"
#include "Logger.h"

// Пересылает события игры в журнал программы: Info — как Game_info,
// Error — как Game_error.
class Game_log_listener : public Game_event_listener {
private:
	Logger& logger_;

public:
	explicit Game_log_listener(Logger& logger) : logger_(logger) {
	}

	void on_game_event(Game_event_type type, const std::string& text) override {
		logger_.log(type == Game_event_type::Error ? Log_level::Game_error : Log_level::Game_info, text);
	}
};

#endif
