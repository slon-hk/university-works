#ifndef OOP_PROJECT_DIRECTION_OFFSET_H
#define OOP_PROJECT_DIRECTION_OFFSET_H

#include "Direction.h"

// Смещение по осям при шаге в заданном направлении (y растёт вниз)
class Direction_offset {
private:
	int dx_;
	int dy_;

public:
	explicit Direction_offset(Direction direction);

	int get_dx() const;
	int get_dy() const;
};

#endif
