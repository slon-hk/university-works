#ifndef OOP_PROJECT_SESSION_SERIALIZER_H
#define OOP_PROJECT_SESSION_SERIALIZER_H

#include <memory>

#include "../../abilities/includes/Ability_catalog.h"
#include "../../game/includes/Game_session.h"
#include "../../items/includes/Item_catalog.h"
#include "Board_serializer.h"
#include "Robot_serializer.h"
#include "Save_reader.h"
#include "Save_writer.h"

// Запись и чтение состояния уровня: ход и награды, сложность, поле, роботы
// со способностями, заводы, инвентарь и журнал. При чтении проверяется,
// что данные согласованы между собой (игрок первый и единственный
// нейтральный, жив и имеет способность и т. п.).
class Session_serializer {
private:
	Board_serializer board_;
	Robot_serializer robot_;
	Ability_catalog abilities_;
	Item_catalog items_;

	void write_difficulty(Save_writer& out, const Difficulty& difficulty) const;
	void write_robots(Save_writer& out, const Game_session& session) const;
	void write_factories(Save_writer& out, const Game_session& session) const;
	void write_inventory(Save_writer& out, const Inventory& inventory) const;
	void write_log(Save_writer& out, const Event_log& log) const;

	Difficulty read_difficulty(Save_reader& in) const;
	std::vector<std::shared_ptr<Robot_abilities>> read_abilities(Save_reader& in) const;
	Robot_factory read_factory(Save_reader& in) const;
	Inventory read_inventory(Save_reader& in) const;

public:
	void write(Save_writer& out, const Game_session& session) const;
	// бросает runtime_error при неверных или несогласованных данных
	std::unique_ptr<Game_session> read(Save_reader& in) const;
};

#endif
