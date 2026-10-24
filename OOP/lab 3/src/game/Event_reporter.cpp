#include "includes/Event_reporter.h"

using namespace std;

// сколько последних записей хранит журнал
const size_t kLogCapacity = 200;

Event_reporter::Event_reporter() : log_(kLogCapacity), listener_(nullptr) {
}

void Event_reporter::notify(Game_event_type type, const string& text) const {
	if (listener_ != nullptr) {
		listener_->on_game_event(type, text);
	}
}

void Event_reporter::info(const string& text) {
	log_.add(text);
	notify(Game_event_type::Info, text);
}

void Event_reporter::error(const string& text) {
	log_.add(text);
	notify(Game_event_type::Error, text);
}

void Event_reporter::trace(const string& text) const {
	notify(Game_event_type::Info, text);
}

void Event_reporter::set_listener(Game_event_listener* listener) {
	listener_ = listener;
}

const Event_log& Event_reporter::get_log() const {
	return log_;
}

void Event_reporter::restore_log(const vector<string>& entries) {
	log_.clear();
	for (size_t i = 0; i < entries.size(); i++) {
		log_.add(entries[i]);
	}
}
