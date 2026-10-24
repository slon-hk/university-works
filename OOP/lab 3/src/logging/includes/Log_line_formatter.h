#ifndef OOP_PROJECT_LOG_LINE_FORMATTER_H
#define OOP_PROJECT_LOG_LINE_FORMATTER_H

#include <string>

#include "Log_level.h"

// Оформляет запись журнала: "2026-10-24 12:00:00 [GAME ERROR   ] сообщение"
class Log_line_formatter {
private:
	std::string get_tag(Log_level level) const;
	std::string get_time() const;

public:
	std::string format(Log_level level, const std::string& message) const;
};

#endif
