#ifndef OOP_PROJECT_INVENTORY_H
#define OOP_PROJECT_INVENTORY_H

#include <memory>
#include <vector>

#include "Item.h"

// Инвентарь игрока ограниченной вместимости. Слоты нумеруются с 0,
// предметы хранятся подряд без пропусков.
class Inventory {
private:
	std::size_t capacity_;
	std::vector<std::shared_ptr<Item>> items_;

public:
	explicit Inventory(std::size_t capacity);

	// false, если инвентарь полон
	bool add_item(std::shared_ptr<Item> item);
	// извлекает предмет из слота (остальные сдвигаются); nullptr при неверном слоте
	std::shared_ptr<Item> take_item(int slot);

	// бросает out_of_range при неверном слоте
	const Item& get_item(int slot) const;
	int get_size() const;
	int get_capacity() const;
	bool is_full() const;
};

#endif //OOP_PROJECT_INVENTORY_H
