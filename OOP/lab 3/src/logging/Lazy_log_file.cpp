#include "includes/Lazy_log_file.h"

using namespace std;

Lazy_log_file::Lazy_log_file(const string& path) : path_(path) {
}

void Lazy_log_file::write(const string& line) {
	if (!file_.is_open()) {
		file_.open(path_, ios::app);
	}
	// сбрасываем буфер сразу, чтобы журнал не терялся при аварийном выходе
	file_ << line << endl;
}
