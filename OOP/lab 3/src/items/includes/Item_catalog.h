#ifndef OOP_PROJECT_ITEM_CATALOG_H
#define OOP_PROJECT_ITEM_CATALOG_H

#include <memory>
#include <string>

#include "Item.h"

// Каталог всех предметов: единственное место, где они перечислены.
class Item_catalog {
public:
	int get_count() const;
	// бросает out_of_range при неверном номере
	std::shared_ptr<Item> create(int index) const;
	// бросает runtime_error, если предмета с таким именем нет
	std::shared_ptr<Item> create(const std::string& name) const;
	std::shared_ptr<Item> create_random() const;
};

#endif
