#include "includes/Random_level_generator.h"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>

#include "includes/Unit_presets.h"

using namespace std;

const int kBoardSize = 32;
const int kPlacementAttempts = 1000;
// враги и заводы появляются не ближе этого расстояния к игроку
const int kSafeDistance = 8;

// Рост сложности с номером уровня (n — номер, начиная с 1).
// стен: 6 + n, но не больше 16
const int kBaseWalls = 6;
const int kMaxWalls = 16;
// врагов: 2 + n, но не больше 12; ранг — n или n + 1
const int kBaseEnemies = 2;
const int kMaxEnemies = 12;
// заводов: (n + 1) / 2, но не больше 4
const int kMaxFactories = 4;
// шансы статусов на клетке, %: 8 + 3(n-1) и 2 + 2(n-1), не больше 25 и 10
const int kBaseOneStatusChance = 8;
const int kOneStatusChancePerLevel = 3;
const int kMaxOneStatusChance = 25;
const int kBaseTwoStatusChance = 2;
const int kTwoStatusChancePerLevel = 2;
const int kMaxTwoStatusChance = 10;

string Random_level_generator::get_name() const {
	return "random";
}

Game_session Random_level_generator::create(int level_number, const Difficulty& difficulty) const {
	int level = max(level_number, 1);
	Unit_presets units(difficulty.get_enemy_power_percent());
	Map_editor editor(kBoardSize, kBoardSize);
	Occupancy used(kBoardSize, vector<bool>(kBoardSize, false));

	editor.set_status_chances(min(kBaseOneStatusChance + kOneStatusChancePerLevel * (level - 1), kMaxOneStatusChance),
							  min(kBaseTwoStatusChance + kTwoStatusChancePerLevel * (level - 1), kMaxTwoStatusChance));
	add_walls(editor, used, min(kBaseWalls + level, kMaxWalls));
	add_slow_areas(editor, 3);

	int player_x;
	int player_y;
	if (!find_free_cell(used, 1, 0, 0, 0, player_x, player_y)) {
		throw runtime_error("Cannot place the player on a random level");
	}
	editor.set_player(Player_state().get_robot(), player_x, player_y);
	mark(used, player_x, player_y, 1);

	int x;
	int y;
	int factories = min((level + 1) / 2, kMaxFactories);
	Robot_factory factory = units.create_factory(level);
	for (int i = 0; i < factories; i++) {
		if (find_free_cell(used, factory.get_size(), player_x, player_y, kSafeDistance, x, y)) {
			editor.add_factory(factory, x, y);
			mark(used, x, y, factory.get_size());
		}
	}
	int enemies = min(kBaseEnemies + level, kMaxEnemies);
	for (int i = 0; i < enemies; i++) {
		if (find_free_cell(used, 1, player_x, player_y, kSafeDistance, x, y)) {
			editor.add_robot(units.create_enemy(level + rand() % 2), x, y);
			mark(used, x, y, 1);
		}
	}
	if (find_free_cell(used, 1, player_x, player_y, 2, x, y)) {
		editor.add_robot(units.create_ally(), x, y);
		mark(used, x, y, 1);
	}
	return editor.build();
}

bool Random_level_generator::find_free_cell(const Occupancy& used, int size, int from_x, int from_y,
											int min_distance, int& out_x, int& out_y) const {
	for (int attempt = 0; attempt < kPlacementAttempts; attempt++) {
		int x = rand() % (kBoardSize - size + 1);
		int y = rand() % (kBoardSize - size + 1);
		// расстояние по большей из осей, как у радиусов в игре
		if (max(abs(x - from_x), abs(y - from_y)) < min_distance) {
			continue;
		}
		bool free = true;
		for (int dy = 0; dy < size; dy++) {
			for (int dx = 0; dx < size; dx++) {
				free = free && !used[y + dy][x + dx];
			}
		}
		if (free) {
			out_x = x;
			out_y = y;
			return true;
		}
	}
	return false;
}

void Random_level_generator::mark(Occupancy& used, int x, int y, int size) const {
	for (int dy = 0; dy < size; dy++) {
		for (int dx = 0; dx < size; dx++) {
			used[y + dy][x + dx] = true;
		}
	}
}

void Random_level_generator::add_walls(Map_editor& editor, Occupancy& used, int count) const {
	for (int i = 0; i < count; i++) {
		int length = 3 + rand() % 4;
		bool horizontal = rand() % 2 == 0;
		int x = rand() % kBoardSize;
		int y = rand() % kBoardSize;
		for (int k = 0; k < length; k++) {
			int wall_x = horizontal ? x + k : x;
			int wall_y = horizontal ? y : y + k;
			if (wall_x < kBoardSize && wall_y < kBoardSize) {
				editor.add_obstacle(wall_x, wall_y);
				used[wall_y][wall_x] = true;
			}
		}
	}
}

void Random_level_generator::add_slow_areas(Map_editor& editor, int count) const {
	for (int i = 0; i < count; i++) {
		int width = 2 + rand() % 4;
		int height = 2 + rand() % 3;
		int x = rand() % (kBoardSize - width);
		int y = rand() % (kBoardSize - height);
		for (int dy = 0; dy < height; dy++) {
			for (int dx = 0; dx < width; dx++) {
				editor.add_slow_cell(x + dx, y + dy);
			}
		}
	}
}
