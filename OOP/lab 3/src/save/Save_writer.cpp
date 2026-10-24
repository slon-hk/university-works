#include "includes/Save_writer.h"

using namespace std;

Save_writer::Save_writer() : line_start_(true) {
}

void Save_writer::separate() {
	if (!line_start_) {
		out_ << ' ';
	}
	line_start_ = false;
}

void Save_writer::write_key(const string& key) {
	separate();
	out_ << key;
}

void Save_writer::write_int(long long value) {
	separate();
	out_ << value;
}

void Save_writer::write_bool(bool value) {
	write_int(value ? 1 : 0);
}

void Save_writer::write_string(const string& text) {
	separate();
	out_ << text.size() << ' ' << text;
}

void Save_writer::end_line() {
	out_ << '\n';
	line_start_ = true;
}

string Save_writer::get_text() const {
	return out_.str();
}
