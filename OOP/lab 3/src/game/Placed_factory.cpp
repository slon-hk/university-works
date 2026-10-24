#include "includes/Placed_factory.h"

Placed_factory::Placed_factory(const Robot_factory& factory, int x, int y) : factory_(factory), x_(x), y_(y) {
}

Robot_factory& Placed_factory::get_factory() {
	return factory_;
}

const Robot_factory& Placed_factory::get_factory() const {
	return factory_;
}

int Placed_factory::get_x() const {
	return x_;
}

int Placed_factory::get_y() const {
	return y_;
}

bool Placed_factory::covers(int x, int y) const {
	int size = factory_.get_size();
	return x >= x_ && x < x_ + size && y >= y_ && y < y_ + size;
}
