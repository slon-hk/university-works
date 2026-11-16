#ifndef OOP_PROJECT_SINGLE_FILE_OUTPUT_H
#define OOP_PROJECT_SINGLE_FILE_OUTPUT_H

#include <string>

#include "Lazy_log_file.h"
#include "Log_level.h"

class Single_file_output {
private:
	Lazy_log_file file_;

public:
	Single_file_output(const std::string& directory);

	void write(Log_level level, const std::string& line);
};

#endif
