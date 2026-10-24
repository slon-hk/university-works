#include "includes/Save_reader.h"

#include <stdexcept>

using namespace std;

// ограничение на длину строки в сохранении
const long long kMaxStringLength = 1000;

Save_reader::Save_reader(const string& text) : in_(text) {
}

void Save_reader::expect(const string& key) {
	string word;
	if (!(in_ >> word) || word != key) {
		throw runtime_error("expected '" + key + "'" + (word.empty() ? "" : ", found '" + word + "'"));
	}
}

long long Save_reader::read_int(long long min, long long max, const string& what) {
	long long value;
	if (!(in_ >> value)) {
		throw runtime_error("cannot read " + what);
	}
	if (value < min || value > max) {
		throw runtime_error(what + " = " + to_string(value) + " is out of range [" + to_string(min) + ", "
							+ to_string(max) + "]");
	}
	return value;
}

bool Save_reader::read_bool(const string& what) {
	return read_int(0, 1, what) == 1;
}

string Save_reader::read_string(const string& what) {
	long long length = read_int(0, kMaxStringLength, what + " length");
	// между длиной и символами ровно один пробел
	if (in_.get() != ' ') {
		throw runtime_error("bad format of " + what);
	}
	string text((size_t) length, '\0');
	if (!in_.read(&text[0], length)) {
		throw runtime_error("unexpected end of file in " + what);
	}
	return text;
}

void Save_reader::expect_end() {
	string rest;
	if (in_ >> rest) {
		throw runtime_error("unexpected data at the end: '" + rest + "'");
	}
}
