#include "includes/File_level_generator.h"

#include <fstream>
#include <stdexcept>
#include <vector>

#include "includes/Map_editor.h"
#include "includes/Unit_presets.h"

using namespace std;


File_level_generator::File_level_generator() : directory_(LEVELS_PATH) {
}

File_level_generator::File_level_generator(const string& directory) : directory_(directory) {
}

string File_level_generator::get_name() const {
	return "fixed";
}

Game_session File_level_generator::create(int level_number, const Difficulty& difficulty) const {
	Unit_presets units(difficulty.get_enemy_power_percent());
	string path = directory_ + "/level_" + to_string(level_number) + ".txt";
	ifstream in(path);
	if (!in) {
		throw runtime_error("Cannot open level file: " + path);
	}
	vector<string> rows;
	string line;
	while (getline(in, line)) {
		// файлы, сохранённые в Windows, оканчивают строки на \r
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		if (!line.empty()) {
			rows.push_back(line);
		}
	}
	if (rows.empty()) {
		throw runtime_error("Level file is empty: " + path);
	}

	int width = (int) rows[0].size();
	int height = (int) rows.size();
	Map_editor editor(width, height);
	editor.set_status_chances(0, 0);
	Cell_status_generator statuses;
	bool has_player = false;

	for (int y = 0; y < height; y++) {
		if ((int) rows[y].size() != width) {
			throw runtime_error("Level rows must have equal length: " + path + ", row " + to_string(y + 1));
		}
		for (int x = 0; x < width; x++) {
			char cell = rows[y][x];
			if (cell == '.') {
				continue;
			} else if (cell == '#') {
				editor.add_obstacle(x, y);
			} else if (cell == '~') {
				editor.add_slow_cell(x, y);
			} else if (cell == 'P') {
				editor.set_player(Player_state().get_robot(), x, y);
				has_player = true;
			} else if (cell == 'A') {
				editor.add_robot(units.create_ally(), x, y);
			} else if (cell == 'E') {
				editor.add_robot(units.create_enemy(1), x, y);
			} else if (cell >= '2' && cell <= '9') {
				editor.add_robot(units.create_enemy(cell - '0'), x, y);
			} else if (cell == 'F') {
				editor.add_factory(units.create_factory(level_number), x, y);
			} else if (cell == 's') {
				editor.add_cell_status(x, y, statuses.create_status(Cell_status_type::Slow));
			} else if (cell == 'o') {
				editor.add_cell_status(x, y, statuses.create_status(Cell_status_type::Overload));
			} else if (cell == 'b') {
				editor.add_cell_status(x, y, statuses.create_status(Cell_status_type::Burn));
			} else if (cell == 'h') {
				editor.add_cell_status(x, y, statuses.create_status(Cell_status_type::Shield));
			} else if (cell == 'i') {
				editor.add_cell_status(x, y, statuses.create_status(Cell_status_type::Item));
			} else {
				throw runtime_error("Unknown symbol '" + string(1, cell) + "' in " + path);
			}
		}
	}
	if (!has_player) {
		throw runtime_error("Level has no player (P): " + path);
	}
	return editor.build();
}
