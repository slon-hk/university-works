#ifndef OOP_PROJECT_FILE_LEVEL_GENERATOR_H
#define OOP_PROJECT_FILE_LEVEL_GENERATOR_H

#include <string>

#include "Difficulty.h"
#include "Game_session.h"

// Фиксированные уровни: карта уровня N читается из файла
// <directory>/level_N.txt. Каждая строка файла — строка поля, символы:
//   .  пустая клетка          #  препятствие         ~  вязкая клетка (3 ОД)
//   P  игрок                  A  союзник             F  завод (левый верхний угол)
//   E  враг 1-го ранга        2..9  враг указанного ранга
//   s  Slow   o  Overload   b  Burn   h  Shield   i  случайный предмет
// Случайные статусы на клетки не добавляются.
class File_level_generator {
private:
	std::string directory_;

public:
	// по умолчанию — каталог levels из исходников (LEVELS_PATH в CMakeLists.txt)
	File_level_generator();
	explicit File_level_generator(const std::string& directory);

	std::string get_name() const;
	// сложность меняет силу врагов и заводов из карты;
	// бросает runtime_error, если файла нет или карта некорректна
	Game_session create(int level_number, const Difficulty& difficulty) const;
};

#endif
