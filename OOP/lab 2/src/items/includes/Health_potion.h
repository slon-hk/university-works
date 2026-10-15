#ifndef OOP_PROJECT_HEALTH_POTION_H
#define OOP_PROJECT_HEALTH_POTION_H

#include "Item.h"

// Зелье здоровья: восстанавливает часть HP.
class Health_potion : public Item {
public:
	std::string get_name() const override;
	std::string get_description() const override;

	bool can_use(const Game_session& session, int user_index) const override;
	void use(Game_session& session, int user_index) const override;
};

#endif //OOP_PROJECT_HEALTH_POTION_H
