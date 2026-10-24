#ifndef OOP_PROJECT_LOGGER_H
#define OOP_PROJECT_LOGGER_H

#include <string>

#include "Log_level.h"

// Интерфейс журнала программы: куда и как писать, решает реализация.
class Logger {
public:
	virtual ~Logger() = default;

	virtual void log(Log_level level, const std::string& message) = 0;
};

#endif
