#ifndef OOP_PROJECT_LAZY_LOG_FILE_H
#define OOP_PROJECT_LAZY_LOG_FILE_H

#include <fstream>
#include <string>

// Файл журнала, открываемый при первой записи: если уровни выключены,
// файл даже не создаётся. Записи дописываются в конец.
class Lazy_log_file {
private:
	std::string path_;
	std::ofstream file_;

public:
	explicit Lazy_log_file(const std::string& path);

	void write(const std::string& line);
};

#endif
