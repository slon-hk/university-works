#ifndef OOP_PROJECT_COMMAND_LINE_OPTIONS_H
#define OOP_PROJECT_COMMAND_LINE_OPTIONS_H

#include <string>

#include "../../logging/includes/Log_settings.h"

// Параметры командной строки. Уровни журнала задаются аргументом
// --log=<список через запятую>, например
//   --log=program_info,game_error   --log=all (по умолчанию)   --log=none
// Уровни: program_info, game_info, program_error, game_error.
// При неверном аргументе остаются все уровни, а описание ошибки
// доступно через get_error().
class Command_line_options {
private:
	Log_settings log_settings_;
	std::string error_;

	void parse_argument(const std::string& argument);

public:
	Command_line_options(int argc, char* argv[]);

	const Log_settings& get_log_settings() const;
	// пустая строка, если все аргументы верны
	const std::string& get_error() const;
};

#endif
