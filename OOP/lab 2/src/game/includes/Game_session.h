#ifndef OOP_PROJECT_GAME_SESSION_H
#define OOP_PROJECT_GAME_SESSION_H

#include <memory>
#include <string>
#include <vector>

#include "../../abilities/includes/Ability_catalog.h"
#include "../../board/includes/Direction.h"
#include "Enemy_controller.h"
#include "Event_log.h"
#include "../../board/includes/Game_board.h"
#include "../../items/includes/Inventory.h"
#include "../../items/includes/Item.h"
#include "../../robot/includes/Robot.h"
#include "../../abilities/includes/Robot_abilities.h"
#include "../../robot/includes/Robot_factory.h"
#include "../../statuses/includes/Robot_status.h"

// Состояние одной игровой партии и все игровые правила: поле, роботы
// (игрок всегда под индексом kPlayerIndex), заводы, инвентарь, журнал.
// Через этот класс способности, предметы и статусы воздействуют на мир.
class Game_session {
private:
	// робот на поле вместе с координатами и способностями
	struct Unit {
		Robot robot;
		int x;
		int y;
		std::vector<std::shared_ptr<Robot_abilities>> abilities;
	};

	// завод и координаты его левого верхнего угла
	struct Factory_placement {
		Robot_factory factory;
		int x;
		int y;
	};

	Game_board board_;
	std::vector<Unit> units_;
	std::vector<Factory_placement> factories_;
	Ability_catalog ability_catalog_;
	Enemy_controller enemy_controller_;
	Inventory inventory_;
	Event_log log_;
	int turn_; // номер текущего хода, начиная с 1
	int rewarded_rank_; // до какого ранга игрок уже получил награды

	Unit& unit(int index);
	const Unit& unit(int index) const;
	// имя робота для лога: "Player", "Enemy #N", "Ally #N"
	std::string unit_name(int index) const;
	// занята ли клетка каким-либо заводом
	bool is_factory_cell(int x, int y) const;
	// ищет свободную клетку вокруг завода для нового робота
	bool find_free_cell_around(const Factory_placement& placement, int& out_x, int& out_y) const;

	// применяет к роботу статусы клетки, на которую он встал
	void enter_cell(int index);
	// Этапы конца хода.
	void process_statuses();
	void spawn_from_factories();
	void restore_robots();
	// пишет в лог потерю HP (с пометкой о щите) и гибель робота
	void log_health_loss(const std::string& prefix, int target_index, int health_before, int damage);

public:
	static const int kPlayerIndex = 0;

	explicit Game_session(Game_board board);

	// Расстановка объектов; бросают runtime_error, если место занято.
	// Первый добавленный робот становится игроком.
	void add_robot(Robot robot, int x, int y);
	void add_factory(Robot_factory factory, int x, int y);

	// ход игрока в указанном направлении
	bool move_player(Direction direction);
	// шаг робота в соседнюю клетку: если там робот — атака или лечение,
	// иначе перемещение; true, если действие выполнено
	bool act(int index, Direction direction);
	// завершает ход: ходят враги, срабатывают статусы, заводы производят
	// роботов, восстанавливаются энергия и очки движения
	void end_turn();
	// пересчитывает видимые игроком клетки
	void update_visibility();
	int get_turn() const;
	// уровень пройден, когда не осталось живых врагов и вражеских заводов
	bool is_level_complete() const;
	// игра окончена, когда погиб игрок
	bool is_game_over() const;

	const Game_board& get_board() const;
	bool is_inside_board(int x, int y) const;
	// внутри поля, не препятствие и не завод
	bool is_passable(int x, int y) const;
	// проходима и не занята живым роботом
	bool is_cell_free(int x, int y) const;
	// попадает ли клетка в квадрат радиуса radius вокруг робота index
	bool is_in_radius(int index, int x, int y, int radius) const;
	int get_robot_count() const;
	const Robot& get_robot(int index) const;
	int get_robot_x(int index) const;
	int get_robot_y(int index) const;
	// индекс живого робота в клетке или -1
	int get_unit_index(int x, int y) const;
	bool is_player(int index) const;

	int get_factory_count() const;
	const Robot_factory& get_factory(int index) const;
	int get_factory_x(int index) const;
	int get_factory_y(int index) const;
	// индекс завода, занимающего клетку, или -1
	int get_factory_index(int x, int y) const;

	// Способности роботов.
	void add_ability(int index, std::shared_ptr<Robot_abilities> ability);
	int get_ability_count(int index) const;
	const Robot_abilities& get_ability(int robot_index, int ability_index) const;
	// индекс способности по имени или -1
	int find_ability(int robot_index, const std::string& name) const;
	// проверяет энергию, применимость и Overload, списывает энергию и
	// применяет способность; true при успехе
	bool use_ability(int user_index, int ability_index, int target_x, int target_y);

	// Воздействия на роботов (используются способностями и предметами),
	// каждое пишет результат в журнал.
	void heal_robot(int index, int heal_amount);
	void restore_energy(int index, int amount);
	void deal_damage(int user_index, int target_index, int damage);
	void move_robot(int index, int x, int y);
	void apply_status(int index, std::shared_ptr<Robot_status> status);
	// урон заводу; разрушенный завод убирается с поля, а атакующий
	// получает опыт
	void damage_factory(int user_index, int factory_index, int damage);

	// Награды за ранг: на каждый новый ранг игрок может выучить новую
	// способность или улучшить имеющуюся.
	bool has_rank_reward() const;
	bool learn_ability(std::shared_ptr<Robot_abilities> ability);
	bool upgrade_ability(int ability_index);

	// Инвентарь игрока.
	bool add_item(std::shared_ptr<Item> item);
	bool use_item(int slot);
	const Inventory& get_inventory() const;

	void log_event(const std::string& text);
	const Event_log& get_log() const;
};

#endif
