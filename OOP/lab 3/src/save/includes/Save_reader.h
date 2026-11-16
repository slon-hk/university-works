#ifndef OOP_PROJECT_SAVE_READER_H
#define OOP_PROJECT_SAVE_READER_H

#include <sstream>
#include <string>

// Чтение текста сохранения по словам с проверкой. Любое несоответствие

class Save_reader {
private:
	std::istringstream in_;

public:
	explicit Save_reader(const std::string& text);

	void expect(const std::string& key);
	long long read_int(long long min, long long max, const std::string& what);
	bool read_bool(const std::string& what);
	std::string read_string(const std::string& what);

	void expect_end();
};

#endif
