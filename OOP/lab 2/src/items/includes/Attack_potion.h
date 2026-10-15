#ifndef OOP_PROJECT_ATTACK_POTION_H
#define OOP_PROJECT_ATTACK_POTION_H

#include "Item.h"

// Зелье атаки: накладывает статус Strength (временный бонус к атаке).
class Attack_potion : public Item {
public:
	std::string get_name() const override;
	std::string get_description() const override;

	bool can_use(const Game_session& session, int user_index) const override;
	void use(Game_session& session, int user_index) const override;
};

#endif //OOP_PROJECT_ATTACK_POTION_H
