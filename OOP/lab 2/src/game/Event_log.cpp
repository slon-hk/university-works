#include "includes/Event_log.h"

using namespace std;

Event_log::Event_log(size_t capacity) : capacity_(capacity > 0 ? capacity : 1) {
}

void Event_log::add(const string& text) {
	entries_.push_back(text);
	if (entries_.size() > capacity_) {
		entries_.erase(entries_.begin());
	}
}

const vector<string>& Event_log::get_entries() const {
	return entries_;
}
