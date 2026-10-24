#include "includes/Item_catalog.h"

#include <cstdlib>
#include <stdexcept>

#include "includes/Attack_potion.h"
#include "includes/Energy_potion.h"
#include "includes/Health_potion.h"

using namespace std;

const int kItemCount = 3;

int Item_catalog::get_count() const {
	return kItemCount;
}

shared_ptr<Item> Item_catalog::create(int index) const {
	if (index == 0) {
		return make_shared<Health_potion>();
	}
	if (index == 1) {
		return make_shared<Energy_potion>();
	}
	if (index == 2) {
		return make_shared<Attack_potion>();
	}
	throw out_of_range("item " + to_string(index));
}

shared_ptr<Item> Item_catalog::create(const string& name) const {
	for (int i = 0; i < kItemCount; i++) {
		shared_ptr<Item> item = create(i);
		if (item->get_name() == name) {
			return item;
		}
	}
	throw runtime_error("unknown item '" + name + "'");
}

shared_ptr<Item> Item_catalog::create_random() const {
	return create(rand() % kItemCount);
}
