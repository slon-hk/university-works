#include "includes/Inventory.h"

#include <stdexcept>
#include <string>

using namespace std;

Inventory::Inventory(size_t capacity) : capacity_(capacity) {
}

bool Inventory::add_item(shared_ptr<Item> item) {
	if (item == nullptr || is_full()) {
		return false;
	}
	items_.push_back(item);
	return true;
}

shared_ptr<Item> Inventory::take_item(int slot) {
	if (slot < 0 || slot >= (int) items_.size()) {
		return nullptr;
	}
	shared_ptr<Item> item = items_[slot];
	items_.erase(items_.begin() + slot);
	return item;
}

const Item& Inventory::get_item(int slot) const {
	if (slot < 0 || slot >= get_size()) {
		throw out_of_range("inventory slot " + to_string(slot));
	}
	return *items_[slot];
}

int Inventory::get_size() const {
	return (int) items_.size();
}

int Inventory::get_capacity() const {
	return (int) capacity_;
}

bool Inventory::is_full() const {
	return items_.size() >= capacity_;
}
