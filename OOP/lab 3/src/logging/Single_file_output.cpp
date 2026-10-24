#include "includes/Single_file_output.h"

using namespace std;

Single_file_output::Single_file_output(const string& directory) : file_(directory + "/game.log") {
}

void Single_file_output::write(Log_level, const string& line) {
	file_.write(line);
}
