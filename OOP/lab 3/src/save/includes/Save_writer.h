#ifndef OOP_PROJECT_SAVE_WRITER_H
#define OOP_PROJECT_SAVE_WRITER_H

#include <sstream>
#include <string>

// Запись текста сохранения по словам — пара к Save_reader. Слова в строке
// разделяются пробелом, строки текста записываются как "<длина> <символы>".
class Save_writer {
private:
	std::ostringstream out_;
	bool line_start_;

	// пробел перед каждым словом, кроме первого в строке
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
