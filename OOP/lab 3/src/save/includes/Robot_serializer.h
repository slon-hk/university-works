#ifndef OOP_PROJECT_ROBOT_SERIALIZER_H
#define OOP_PROJECT_ROBOT_SERIALIZER_H

#include "../../robot/includes/Robot.h"
#include "Save_reader.h"
#include "Save_writer.h"
#include "Status_serializer.h"


class Robot_serializer {
private:
	Status_serializer statuses_;

public:
	void write(Save_writer& out, const Robot& robot) const;
	Robot read(Save_reader& in) const;
};

#endif
