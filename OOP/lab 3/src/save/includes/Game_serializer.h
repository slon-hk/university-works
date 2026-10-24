#ifndef OOP_PROJECT_GAME_SERIALIZER_H
#define OOP_PROJECT_GAME_SERIALIZER_H

#include <cstdint>
#include <string>

#include "../../game/includes/Game.h"
#include "Loaded_game.h"
#include "Session_serializer.h"

// Система сохранения и загрузки. Не встроена в игру: читает её состояние
// через публичный интерфейс Game / Game_session и собирает новое состояние,
// которое игра принимает через Game::restore().
//
// Сам отвечает за файл: заголовок с версией, контрольную сумму, режим,
// уровень и выбор способности; состояние уровня (поле, роботы, заводы,
// инвентарь, журнал, сложность) записывает и проверяет Session_serializer.
class Game_serializer {
private:
	Session_serializer session_;

	// FNV-1a
	std::uint64_t checksum(const std::string& text) const;
	std::string write_body(const Game& game) const;
	Loaded_game read_body(const std::string& body) const;

public:
	// Бросает runtime_error, если игрок сейчас не может действовать или
	// файл не удалось записать; прежнее сохранение при этом не портится.
	void save(const Game& game, const std::string& path) const;
	// Читает и проверяет файл; бросает runtime_error с описанием ошибки.
	// Ничего, кроме возвращаемого объекта, не меняет.
	Loaded_game load(const std::string& path) const;

	bool exists(const std::string& path) const;
};

#endif
