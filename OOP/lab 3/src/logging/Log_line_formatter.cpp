#include "includes/Log_line_formatter.h"

#include <ctime>

using namespace std;

string Log_line_formatter::format(Log_level level, const string& message) const {
	return get_time() + " [" + get_tag(level) + "] " + message;
}

string Log_line_formatter::get_tag(Log_level level) const {
	if (level == Log_level::Program_info) {
		return "PROGRAM INFO ";
	}
	if (level == Log_level::Game_info) {
		return "GAME INFO    ";
	}
	if (level == Log_level::Program_error) {
		return "PROGRAM ERROR";
	}
	return "GAME ERROR   ";
}

string Log_line_formatter::get_time() const {
	time_t now = time(nullptr);
	char buffer[32];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
	return buffer;
}
