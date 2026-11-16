#ifndef OOP_PROJECT_GAME_SERIALIZER_H
#define OOP_PROJECT_GAME_SERIALIZER_H

#include <cstdint>
#include <string>

#include "../../game/includes/Game.h"
#include "Loaded_game.h"
#include "Session_serializer.h"

// Система сохранения и загрузки.
class Game_serializer {
private:
	Session_serializer session_;

	std::uint64_t checksum(const std::string& text) const;
	std::string write_body(const Game& game) const;
	Loaded_game read_body(const std::string& body) const;

public:
	// Бросает runtime_error, если игрок сейчас не может действовать или
	// файл не удалось записать;
	void save(const Game& game, const std::string& path) const;
	// Читает и проверяет файл; бросает runtime_error с описанием ошибки.
	Loaded_game load(const std::string& path) const;

	bool exists(const std::string& path) const;
};

#endif
