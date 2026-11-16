#ifndef OOP_PROJECT_PROGRAM_H
#define OOP_PROJECT_PROGRAM_H

#include "../../logging/includes/File_logger.h"
#include "Command_line_options.h"

class Program {
private:
	using Program_logger = File_logger<Split_file_output>;

	Command_line_options options_;

	void run_loop(Program_logger& logger);

public:
	Program(int argc, char* argv[]);

	void run();
};

#endif
