#ifndef OOP_PROJECT_SAVE_WRITER_H
#define OOP_PROJECT_SAVE_WRITER_H

#include <sstream>
#include <string>

class Save_writer {
private:
	std::ostringstream out_;
	bool line_start_;

	void separate();

public:
	Save_writer();

	void write_key(const std::string& key);
	void write_int(long long value);
	void write_bool(bool value);
	void write_string(const std::string& text);
	void end_line();

	std::string get_text() const;
};

#endif
