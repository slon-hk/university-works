#ifndef OOP_PROJECT_EVENT_LOG_H
#define OOP_PROJECT_EVENT_LOG_H

#include <cstddef>
#include <string>
#include <vector>

// Журнал событий игры, показываемый в боковой панели.
// Хранит не более capacity_ последних записей.
class Event_log {
private:
	std::size_t capacity_;
	std::vector<std::string> entries_;

public:
	explicit Event_log(std::size_t capacity);

	// добавляет запись; при переполнении удаляется самая старая
	void add(const std::string& text);
	void clear();
	std::size_t get_capacity() const;
	const std::vector<std::string>& get_entries() const;
};

#endif
