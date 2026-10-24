#ifndef OOP_PROJECT_SAVE_SLOT_H
#define OOP_PROJECT_SAVE_SLOT_H

#include <memory>
#include <string>

#include "../../save/includes/Game_serializer.h"
#include "Game.h"
#include "Game_event_listener.h"
#include "Game_mode_catalog.h"

// Файл сохранения: запись текущей игры и сборка игры нужного режима
// из прочитанного состояния.
class Save_slot {
private:
	std::string path_;
	Game_serializer serializer_;
	Game_mode_catalog modes_;

public:
	explicit Save_slot(const std::string& path);

	const std::string& get_path() const;
	bool exists() const;
	// бросает runtime_error, если сохранить нельзя; прежний файл не портится
	void save(const Game& game) const;
	// бросает runtime_error при ошибке чтения или проверки, ничего не меняя
	std::unique_ptr<Game> load(Game_event_listener* listener) const;
};

#endif
