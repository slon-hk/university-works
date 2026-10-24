#ifndef OOP_PROJECT_SPLIT_FILE_OUTPUT_H
#define OOP_PROJECT_SPLIT_FILE_OUTPUT_H

#include <string>

#include "Lazy_log_file.h"
#include "Log_level.h"

// Вывод журнала: информация — в <directory>/info.log, ошибки — в <directory>/errors.log
class Split_file_output {
private:
	Lazy_log_file info_;
	Lazy_log_file errors_;

public:
	explicit Split_file_output(const std::string& directory);

	void write(Log_level level, const std::string& line);
};

#endif
