#ifndef OOP_PROJECT_FILE_LEVEL_GENERATOR_H
#define OOP_PROJECT_FILE_LEVEL_GENERATOR_H

#include <string>

#include "Difficulty.h"
#include "Game_session.h"

// Фиксированные уровни:
// <directory>/level_N.txt
//   .  пустая клетка          #  препятствие         ~  вязкая клетка (3 ОД)
//   P  игрок                  A  союзник             F  завод (левый верхний угол)
//   E  враг 1-го ранга        2..9  враг указанного ранга
//   s  Slow   o  Overload   b  Burn   h  Shield   i  случайный предмет

class File_level_generator {
private:
	std::string directory_;

public:
	File_level_generator();
	File_level_generator(const std::string& directory);

	std::string get_name() const;
	Game_session create(int level_number, const Difficulty& difficulty) const;
};

#endif
