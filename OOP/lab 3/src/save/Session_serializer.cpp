#include "includes/Session_serializer.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "../game/includes/Player_state.h"
#include "../game/includes/Robot_unit.h"

using namespace std;

// Границы проверяемых значений
const long long kMaxTurns = 1000000;
const long long kMaxFactoryLevel = 1000000;
const long long kMaxRobots = 10000;
const long long kMaxFactoryCount = 1000;
const long long kMaxRobotsPerSpawn = 100;
const long long kMaxSpawnPeriod = 1000;

// ---------- запись

void Session_serializer::write(Save_writer& out, const Game_session& session) const {
	out.write_key("progress");
	out.write_int(session.get_turn());
	out.write_int(session.get_rewarded_rank());
	out.write_bool(session.has_player_used_ability());
	out.end_line();
	write_difficulty(out, session.get_difficulty());
	board_.write(out, session.get_board());
	write_robots(out, session);
	write_factories(out, session);
	write_inventory(out, session.get_inventory());
	write_log(out, session.get_log());
}

void Session_serializer::write_difficulty(Save_writer& out, const Difficulty& difficulty) const {
	out.write_key("difficulty");
	out.write_int((int) difficulty.get_level());
	out.write_int(difficulty.get_modifiers().size());
	for (Modifier modifier : difficulty.get_modifiers()) {
		out.write_int((int) modifier);
	}
	out.end_line();
}

void Session_serializer::write_robots(Save_writer& out, const Game_session& session) const {
	out.write_key("robots");
	out.write_int(session.get_robots().get_count());
	out.end_line();
	for (int i = 0; i < session.get_robots().get_count(); i++) {
		out.write_key("robot");
		out.write_int(session.get_robots().get_x(i));
		out.write_int(session.get_robots().get_y(i));
		robot_.write(out, session.get_robots().get_robot(i));
		out.write_key("abilities");
		out.write_int(session.get_robots().get_ability_count(i));
		out.end_line();
		for (int k = 0; k < session.get_robots().get_ability_count(i); k++) {
			const Robot_abilities& ability = session.get_robots().get_ability(i, k);
			out.write_int(ability.get_level());
			out.write_string(ability.get_name());
			out.end_line();
		}
	}
}

void Session_serializer::write_factories(Save_writer& out, const Game_session& session) const {
	out.write_key("factories");
	out.write_int(session.get_factories().get_count());
	out.end_line();
	for (int i = 0; i < session.get_factories().get_count(); i++) {
		const Robot_factory& factory = session.get_factories().get_factory(i);
		out.write_key("factory");
		out.write_int(session.get_factories().get_x(i));
		out.write_int(session.get_factories().get_y(i));
		out.write_int(factory.get_factory_output());
		out.write_int(factory.get_level());
		out.write_int(factory.get_spawn_period());
		out.write_int(factory.get_turns_until_spawn());
		out.write_int(factory.get_current_health());
		out.write_int(factory.get_max_health());
		robot_.write(out, factory.get_unit());
	}
}

void Session_serializer::write_inventory(Save_writer& out, const Inventory& inventory) const {
	out.write_key("inventory");
	out.write_int(inventory.get_size());
	out.end_line();
	for (int i = 0; i < inventory.get_size(); i++) {
		out.write_string(inventory.get_item(i).get_name());
		out.end_line();
	}
}

void Session_serializer::write_log(Save_writer& out, const Event_log& log) const {
	const vector<string>& entries = log.get_entries();
	out.write_key("log");
	out.write_int(entries.size());
	out.end_line();
	for (size_t i = 0; i < entries.size(); i++) {
		out.write_string(entries[i]);
		out.end_line();
	}
}

// ---------- чтение

unique_ptr<Game_session> Session_serializer::read(Save_reader& in) const {
	in.expect("progress");
	int turn = (int) in.read_int(1, kMaxTurns, "turn");
	int rewarded_rank = (int) in.read_int(1, Robot::kMaxStat, "rewarded rank");
	bool player_used_ability = in.read_bool("ability used this turn");
	Difficulty difficulty = read_difficulty(in);

	unique_ptr<Game_session> session = make_unique<Game_session>(board_.read(in));
	session->set_difficulty(difficulty);

	// роботы читаются до заводов (так они записаны), а добавляются после,
	// чтобы проверка клеток учитывала заводы
	vector<Robot_unit> robots;
	in.expect("robots");
	long long robot_count = in.read_int(1, kMaxRobots, "robot count");
	for (long long i = 0; i < robot_count; i++) {
		in.expect("robot");
		int x = (int) in.read_int(0, Game_board::kMaxSize - 1, "robot x");
		int y = (int) in.read_int(0, Game_board::kMaxSize - 1, "robot y");
		Robot robot = robot_.read(in);
		vector<shared_ptr<Robot_abilities>> abilities = read_abilities(in);
		// игрок — всегда первый робот и единственный нейтральный
		bool is_player = i == Game_session::kPlayerIndex;
		if (is_player != (robot.get_relation() == Relation::Neutral)) {
			throw runtime_error("robot #" + to_string(i) + " has wrong relation");
		}
		robots.push_back(Robot_unit(robot, x, y, abilities));
	}
	const Robot& player = robots[Game_session::kPlayerIndex].get_robot();
	if (!player.is_alive() || robots[Game_session::kPlayerIndex].get_abilities().empty()) {
		throw runtime_error("player must be alive and have an ability");
	}
	// награды могут отставать от ранга, только если выбрать нечего
	// (это проверит Game::restore через has_rank_reward)
	if (rewarded_rank > (int) player.get_rank()) {
		throw runtime_error("rewarded rank exceeds player rank");
	}

	in.expect("factories");
	long long factory_count = in.read_int(0, kMaxFactoryCount, "factory count");
	for (long long i = 0; i < factory_count; i++) {
		in.expect("factory");
		int x = (int) in.read_int(0, Game_board::kMaxSize - 1, "factory x");
		int y = (int) in.read_int(0, Game_board::kMaxSize - 1, "factory y");
		session->add_factory(read_factory(in), x, y);
	}
	for (size_t i = 0; i < robots.size(); i++) {
		session->restore_robot(robots[i].get_robot(), robots[i].get_x(), robots[i].get_y(),
							   robots[i].get_abilities());
	}

	Inventory inventory = read_inventory(in);

	in.expect("log");
	long long log_count = in.read_int(0, (long long) session->get_log().get_capacity(), "log size");
	vector<string> log;
	for (long long i = 0; i < log_count; i++) {
		log.push_back(in.read_string("log entry"));
	}

	session->restore_progress(turn, rewarded_rank, player_used_ability);
	session->restore_inventory(inventory);
	session->restore_log(log);
	session->update_visibility();
	return session;
}

Difficulty Session_serializer::read_difficulty(Save_reader& in) const {
	in.expect("difficulty");
	Difficulty_level level = (Difficulty_level) in.read_int(0, kDifficultyLevelCount - 1, "difficulty");
	long long modifier_count = in.read_int(0, kModifierCount, "modifier count");
	vector<Modifier> modifiers;
	for (long long i = 0; i < modifier_count; i++) {
		Modifier modifier = (Modifier) in.read_int(0, kModifierCount - 1, "modifier");
		if (find(modifiers.begin(), modifiers.end(), modifier) != modifiers.end()) {
			throw runtime_error("modifier is listed twice");
		}
		modifiers.push_back(modifier);
	}
	return Difficulty(level, modifiers);
}

vector<shared_ptr<Robot_abilities>> Session_serializer::read_abilities(Save_reader& in) const {
	in.expect("abilities");
	long long count = in.read_int(0, abilities_.get_count(), "ability count");
	vector<shared_ptr<Robot_abilities>> abilities;
	for (long long i = 0; i < count; i++) {
		int level = (int) in.read_int(1, Ability_catalog::kMaxLevel, "ability level");
		abilities.push_back(abilities_.create(in.read_string("ability name"), level));
	}
	return abilities;
}

Robot_factory Session_serializer::read_factory(Save_reader& in) const {
	size_t output = (size_t) in.read_int(1, kMaxRobotsPerSpawn, "factory output");
	size_t level = (size_t) in.read_int(1, kMaxFactoryLevel, "factory level");
	size_t period = (size_t) in.read_int(1, kMaxSpawnPeriod, "factory spawn period");
	size_t turns = (size_t) in.read_int(1, (long long) period, "factory turns until spawn");
	long long health = in.read_int(1, Robot::kMaxStat, "factory health");
	size_t max_health = (size_t) in.read_int(health, Robot::kMaxStat, "factory max health");
	Robot unit = robot_.read(in);
	if (unit.get_relation() == Relation::Neutral) {
		throw runtime_error("factory unit cannot be neutral");
	}
	Robot_factory factory(unit, output, level, period, max_health);
	factory.set_turns_until_spawn(turns);
	factory.take_damage((int) (max_health - health));
	return factory;
}

Inventory Session_serializer::read_inventory(Save_reader& in) const {
	in.expect("inventory");
	Inventory inventory(Player_state::kInventoryCapacity);
	long long count = in.read_int(0, inventory.get_capacity(), "item count");
	for (long long i = 0; i < count; i++) {
		inventory.add_item(items_.create(in.read_string("item name")));
	}
	return inventory;
}
