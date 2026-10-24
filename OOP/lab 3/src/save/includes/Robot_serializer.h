#ifndef OOP_PROJECT_ROBOT_SERIALIZER_H
#define OOP_PROJECT_ROBOT_SERIALIZER_H

#include "../../robot/includes/Robot.h"
#include "Save_reader.h"
#include "Save_writer.h"
#include "Status_serializer.h"

// Запись и чтение робота: характеристики (атака — без бонусов статусов),
// очки движения, отношение и статусы. Значения проверяются на согласованность.
class Robot_serializer {
private:
	Status_serializer statuses_;

public:
	// характеристики дописываются в текущую строку, статусы — следующими строками
	void write(Save_writer& out, const Robot& robot) const;
	Robot read(Save_reader& in) const;
};

#endif
