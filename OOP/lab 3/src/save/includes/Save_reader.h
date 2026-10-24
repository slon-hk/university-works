#ifndef OOP_PROJECT_SAVE_READER_H
#define OOP_PROJECT_SAVE_READER_H

#include <sstream>
#include <string>

// Чтение текста сохранения по словам с проверкой. Любое несоответствие
// (не то слово, число вне диапазона, обрыв данных) — runtime_error
// с описанием, что именно не так.
// Строки записываются как "<длина> <символы>", поэтому могут содержать пробелы.
class Save_reader {
private:
	std::istringstream in_;

public:
	explicit Save_reader(const std::string& text);

	// следующее слово должно быть равно key
	void expect(const std::string& key);
	// целое в [min, max]; what — что читаем (для сообщения об ошибке)
	long long read_int(long long min, long long max, const std::string& what);
	bool read_bool(const std::string& what);
	std::string read_string(const std::string& what);
	// после данных ничего не должно остаться
	void expect_end();
};

#endif
