#ifndef OOP_PROJECT_FILE_LOGGER_H
#define OOP_PROJECT_FILE_LOGGER_H

#include <string>

#include "Log_line_formatter.h"
#include "Log_settings.h"
#include "Logger.h"
#include "Single_file_output.h"
#include "Split_file_output.h"

template <typename Output>
class File_logger : public Logger {
private:
	Log_settings settings_;
	Output output_;

	Log_line_formatter formatter_;

public:
	File_logger(const Log_settings& settings, const std::string& directory)
		: settings_(settings), output_(directory) {
	}

	void log(Log_level level, const std::string& message) override {
		if (settings_.is_enabled(level)) {
			output_.write(level, formatter_.format(level, message));
		}
	}
};

#endif
