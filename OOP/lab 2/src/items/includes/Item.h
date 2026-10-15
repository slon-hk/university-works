#ifndef OOP_PROJECT_ITEM_H
#define OOP_PROJECT_ITEM_H

#include <string>

class Game_session;

// Интерфейс предмета инвентаря. Предмет одноразовый: после use()
// он удаляется из инвентаря (это делает Game_session).
class Item {
public:
	virtual ~Item() = default;

	virtual std::string get_name() const = 0;
	virtual std::string get_description() const = 0;

	// можно ли сейчас применить предмет к роботу user_index
	// (например, зелье здоровья бесполезно при полном HP)
	virtual bool can_use(const Game_session& session, int user_index) const = 0;
	// применяет эффект предмета
	virtual void use(Game_session& session, int user_index) const = 0;
};

#endif //OOP_PROJECT_ITEM_H
