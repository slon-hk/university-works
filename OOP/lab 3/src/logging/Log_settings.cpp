#include "includes/Log_settings.h"

#include <sstream>
#include <stdexcept>

using namespace std;

const char* const kLevelNames[kLogLevelCount] = { "program_info", "game_info", "program_error", "game_error" };

Log_settings::Log_settings() {
	for (bool& enabled : enabled_) {
		enabled = true;
	}
}

Log_settings::Log_settings(const string& levels) : Log_settings() {
	if (levels == "all") {
		return;
	}
	for (bool& enabled : enabled_) {
		enabled = false;
	}
	if (levels == "none") {
		return;
	}
	stringstream list(levels);
	string name;
	while (getline(list, name, ',')) {
		int index = 0;
		while (index < kLogLevelCount && name != kLevelNames[index]) {
			index++;
		}
		if (index == kLogLevelCount) {
			throw runtime_error("unknown log level '" + name + "'");
		}
		enabled_[index] = true;
	}
}

bool Log_settings::is_enabled(Log_level level) const {
	return enabled_[(int) level];
}

string Log_settings::to_string() const {
	string result;
	for (int i = 0; i < kLogLevelCount; i++) {
		if (enabled_[i]) {
			result += (result.empty() ? "" : ",") + string(kLevelNames[i]);
		}
	}
	return result.empty() ? "none" : result;
}
