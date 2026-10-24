#ifndef OOP_PROJECT_LOG_LEVEL_H
#define OOP_PROJECT_LOG_LEVEL_H

// Уровни журнала программы
enum class Log_level {
	Program_info, // загрузка, сохранение, новая игра, выход в меню...
	Game_info, // ходы, перемещения, способности, ранги, уровни
	Program_error, // не удалось загрузить/сохранить, неверные файлы
	Game_error, // недопустимые действия в игре
};

const int kLogLevelCount = 4;

#endif
