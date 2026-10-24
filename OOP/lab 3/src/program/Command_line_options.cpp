#include "includes/Command_line_options.h"

#include <stdexcept>

using namespace std;

const string kLogArgument = "--log=";

Command_line_options::Command_line_options(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		try {
			parse_argument(argv[i]);
		} catch (const exception& error) {
			log_settings_ = Log_settings();
			error_ = error.what();
		}
	}
}

void Command_line_options::parse_argument(const string& argument) {
	if (argument.rfind(kLogArgument, 0) != 0) {
		throw runtime_error("unknown argument '" + argument + "'");
	}
	log_settings_ = Log_settings(argument.substr(kLogArgument.size()));
}

const Log_settings& Command_line_options::get_log_settings() const {
	return log_settings_;
}

const string& Command_line_options::get_error() const {
	return error_;
}
