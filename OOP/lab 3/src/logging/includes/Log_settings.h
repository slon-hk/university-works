#ifndef OOP_PROJECT_LOG_SETTINGS_H
#define OOP_PROJECT_LOG_SETTINGS_H

#include <string>

#include "Log_level.h"

// Набор включённых уровней логирования. Задаётся строкой вида
// "program_info,game_error"; "all" — все уровни, "none" — ни одного.
class Log_settings {
private:
	bool enabled_[kLogLevelCount];

public:
	// все уровни включены
	Log_settings();
	// бросает runtime_error при неизвестном имени уровня
	explicit Log_settings(const std::string& levels);

	bool is_enabled(Log_level level) const;
	// например "program_info,game_error" или "none"
	std::string to_string() const;
};

#endif
