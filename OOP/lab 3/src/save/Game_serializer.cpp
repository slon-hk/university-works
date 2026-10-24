#include "includes/Game_serializer.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../abilities/includes/Ability_catalog.h"
#include "includes/Save_reader.h"
#include "includes/Save_writer.h"

using namespace std;

// Формат файла:
//   ROBOT_GAME_SAVE <версия>
//   checksum <FNV-1a от всего, что ниже>
//   mode, level, controller, состояние уровня (см. Session_serializer), end
// Строки записываются как "<длина> <символы>".
const string kHeader = "ROBOT_GAME_SAVE";
const int kVersion = 2;
const long long kMaxLevel = 1000000;

uint64_t Game_serializer::checksum(const string& text) const {
	uint64_t hash = 14695981039346656037ull;
	for (unsigned char symbol : text) {
		hash ^= symbol;
		hash *= 1099511628211ull;
	}
	return hash;
}

// ---------- сохранение

string Game_serializer::write_body(const Game& game) const {
	Save_writer out;
	out.write_key("mode");
	out.write_string(game.get_mode_name());
	out.end_line();
	out.write_key("level");
	out.write_int(game.get_level());
	out.end_line();
	out.write_key("controller");
	out.write_int(game.get_controller().get_selected_slot());
	out.write_bool(game.get_controller().is_ability_switched());
	out.end_line();
	session_.write(out, game.get_session());
	out.write_key("end");
	out.end_line();
	return out.get_text();
}

void Game_serializer::save(const Game& game, const string& path) const {
	if (!game.can_act()) {
		throw runtime_error("the game can be saved only when the player can act");
	}
	string body = write_body(game);

	// пишем во временный файл и подменяем им сохранение, чтобы сбой
	// записи не испортил прежний файл
	string temp_path = path + ".tmp";
	{
		ofstream out(temp_path, ios::binary);
		out << kHeader << ' ' << kVersion << '\n' << "checksum " << checksum(body) << '\n' << body;
		if (!out) {
			std::remove(temp_path.c_str());
			throw runtime_error("cannot write " + temp_path);
		}
	}
	if (std::rename(temp_path.c_str(), path.c_str()) != 0) {
		std::remove(temp_path.c_str());
		throw runtime_error("cannot replace " + path);
	}
}

// ---------- загрузка

Loaded_game Game_serializer::read_body(const string& body) const {
	Save_reader in(body);
	in.expect("mode");
	string mode = in.read_string("mode");
	in.expect("level");
	int level = (int) in.read_int(1, kMaxLevel, "level");
	in.expect("controller");
	int selected_slot = (int) in.read_int(0, Ability_catalog().get_count() - 1, "selected ability slot");
	bool ability_switched = in.read_bool("ability switched");
	unique_ptr<Game_session> session = session_.read(in);
	in.expect("end");
	in.expect_end();
	return Loaded_game(mode, level, std::move(session), selected_slot, ability_switched);
}

Loaded_game Game_serializer::load(const string& path) const {
	ifstream in(path, ios::binary);
	if (!in) {
		throw runtime_error("cannot open " + path);
	}
	string header;
	int version = 0;
	string checksum_key;
	uint64_t expected = 0;
	if (!(in >> header >> version) || header != kHeader) {
		throw runtime_error("not a save file");
	}
	if (version != kVersion) {
		throw runtime_error("unsupported save version " + to_string(version));
	}
	if (!(in >> checksum_key >> expected) || checksum_key != "checksum" || in.get() != '\n') {
		throw runtime_error("checksum is missing");
	}
	ostringstream rest;
	rest << in.rdbuf();
	string body = rest.str();
	if (checksum(body) != expected) {
		throw runtime_error("checksum mismatch: the file is damaged or was edited");
	}
	try {
		return read_body(body);
	} catch (const exception& error) {
		throw runtime_error(string("invalid save data: ") + error.what());
	}
}

bool Game_serializer::exists(const string& path) const {
	ifstream in(path);
	return (bool) in;
}
