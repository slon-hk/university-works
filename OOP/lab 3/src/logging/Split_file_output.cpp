#include "includes/Split_file_output.h"

using namespace std;

Split_file_output::Split_file_output(const string& directory)
	: info_(directory + "/info.log"), errors_(directory + "/errors.log") {
}

void Split_file_output::write(Log_level level, const string& line) {
	bool is_error = level == Log_level::Program_error || level == Log_level::Game_error;
	(is_error ? errors_ : info_).write(line);
}
