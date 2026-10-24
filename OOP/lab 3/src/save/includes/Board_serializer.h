#ifndef OOP_PROJECT_BOARD_SERIALIZER_H
#define OOP_PROJECT_BOARD_SERIALIZER_H

#include "../../board/includes/Game_board.h"
#include "Save_reader.h"
#include "Save_writer.h"
#include "Status_serializer.h"

// Запись и чтение поля: размер и по строке на каждую клетку (стоимость
// прохода, препятствие, разведана ли) со статусами клетки.
class Board_serializer {
private:
	Status_serializer statuses_;

public:
	void write(Save_writer& out, const Game_board& board) const;
	Game_board read(Save_reader& in) const;
};

#endif
