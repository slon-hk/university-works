#ifndef OOP_PROJECT_STATUS_SERIALIZER_H
#define OOP_PROJECT_STATUS_SERIALIZER_H

#include <memory>

#include "../../items/includes/Item_catalog.h"
#include "../../statuses/includes/Robot_status.h"
#include "Save_reader.h"
#include "Save_writer.h"

class Status_serializer {
private:
	Item_catalog items_;

public:
	void write(Save_writer& out, const Robot_status& status) const;
	std::shared_ptr<Robot_status> read(Save_reader& in, bool on_robot) const;
};

#endif
