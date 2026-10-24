#ifndef OOP_PROJECT_EVENT_REPORTER_H
#define OOP_PROJECT_EVENT_REPORTER_H

#include <string>
#include <vector>

#include "Event_log.h"
#include "Game_event_listener.h"

// Куда уходят сообщения о событиях партии: info и error попадают
// в журнал на экране и к слушателю, trace — только к слушателю
// (подробности вроде каждого шага, которые засорили бы журнал).
class Event_reporter {
private:
	Event_log log_;
	Game_event_listener* listener_; // не владеет; может быть nullptr

	void notify(Game_event_type type, const std::string& text) const;

public:
	Event_reporter();

	void info(const std::string& text);
	void error(const std::string& text);
	void trace(const std::string& text) const;

	// слушатель событий (не владеет им); nullptr — без слушателя
	void set_listener(Game_event_listener* listener);
	const Event_log& get_log() const;
	// заменяет журнал; лишние старые записи отбрасываются
	void restore_log(const std::vector<std::string>& entries);
};

#endif
