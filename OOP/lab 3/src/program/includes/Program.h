#ifndef OOP_PROJECT_PROGRAM_H
#define OOP_PROJECT_PROGRAM_H

#include "../../logging/includes/File_logger.h"
#include "Command_line_options.h"

// Вся программа целиком: собирает журнал, ввод, визуализацию и приложение
// и крутит главный цикл, пока приложение работает.
class Program {
private:
	// Куда пишется журнал: Single_file_output — всё в game.log,
	// Split_file_output — информация в info.log, ошибки в errors.log
	using Program_logger = File_logger<Split_file_output>;

	Command_line_options options_;

	void run_loop(Program_logger& logger);

public:
	Program(int argc, char* argv[]);

	void run();
};

#endif
