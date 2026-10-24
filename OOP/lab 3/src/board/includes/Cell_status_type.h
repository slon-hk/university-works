#ifndef OOP_PROJECT_CELL_STATUS_TYPE_H
#define OOP_PROJECT_CELL_STATUS_TYPE_H

// Виды статусов, которые могут лежать на клетке
enum class Cell_status_type {
	Slow,
	Overload,
	Burn,
	Shield,
	Item
};

const int kCellStatusTypeCount = 5;

#endif
