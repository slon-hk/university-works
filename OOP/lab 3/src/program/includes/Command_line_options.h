#ifndef OOP_PROJECT_COMMAND_LINE_OPTIONS_H
#define OOP_PROJECT_COMMAND_LINE_OPTIONS_H

#include <string>

#include "../../logging/includes/Log_settings.h"

class Command_line_options {
private:
	Log_settings log_settings_;
	std::string error_;

	void parse_argument(const std::string& argument);

public:
	Command_line_options(int argc, char* argv[]);

	const Log_settings& get_log_settings() const;
	const std::string& get_error() const;
};

#endif
