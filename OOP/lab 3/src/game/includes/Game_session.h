#ifndef OOP_PROJECT_GAME_SESSION_H
#define OOP_PROJECT_GAME_SESSION_H

#include <memory>
#include <string>
#include <vector>

#include "../../abilities/includes/Robot_abilities.h"
#include "../../board/includes/Direction.h"
#include "../../board/includes/Game_board.h"
#include "../../items/includes/Inventory.h"
#include "../../items/includes/Item.h"
#include "../../robot/includes/Robot.h"
#include "../../robot/includes/Robot_factory.h"
#include "../../statuses/includes/Robot_status.h"
#include "Difficulty.h"
#include "Enemy_controller.h"
#include "Event_log.h"
#include "Event_reporter.h"
#include "Factory_roster.h"
#include "Game_event_listener.h"
#include "Player_state.h"
#include "Rank_rewards.h"
#include "Robot_roster.h"

// Состояние одной игровой партии и игровые правила: перемещение
// и взаимодействие роботов, конец хода, воздействия способностей,
// предметов и статусов на мир. Хранение роботов, заводов, журнала
// и наград за ранг вынесено в отдельные классы; читать их можно через
// get_robots() / get_factories(), а менять — только действиями ниже,
// чтобы каждое изменение попало в журнал.
class Game_session {
private:
	Game_board board_;
	Robot_roster robots_;
	Factory_roster factories_;
	Ability_catalog ability_catalog_;
	Enemy_controller enemy_controller_;
	Inventory inventory_;
	Event_reporter reporter_;
	Rank_rewards rewards_;
	int turn_; // номер текущего хода, начиная с 1
	Difficulty difficulty_; // модификаторы меняют правила ниже
	bool player_used_ability_; // игрок уже применил способность в этом ходу

	// ищет свободную клетку вокруг завода для нового робота
	bool find_free_cell_around(int factory_index, int& out_x, int& out_y) const;
	// шаг в клетку завода — атака, если завод вражеский
	bool attack_factory(int index, int factory_index, const std::string& target_cell);
	// шаг в клетку с роботом — атака врага или лечение союзника
	bool interact(int index, int target_index);
	// перемещение в соседнюю свободную клетку
	bool step(int index, int x, int y, const std::string& target_cell);

	// применяет к роботу статусы клетки, на которую он встал
	void enter_cell(int index);
	// Этапы конца хода.
	void process_statuses();
	void spawn_from_factories();
	void restore_robots();
	// модификатор Random_status_each_turn
	void apply_random_status_to_player();
	// сообщает о повышении ранга, если он вырос с rank_before
	void report_rank_up(int index, std::size_t rank_before);
	// пишет в лог потерю HP (с пометкой о щите) и гибель робота
	void log_health_loss(const std::string& prefix, int target_index, int health_before, int damage);

public:
	static constexpr int kPlayerIndex = Robot_roster::kPlayerIndex;

	explicit Game_session(Game_board board);

	// Расстановка объектов; бросают runtime_error, если место занято.
	// Первый добавленный робот становится игроком.
	void add_robot(Robot robot, int x, int y);
	void add_factory(Robot_factory factory, int x, int y);

	// Восстановление состояния (используется системой загрузки).
	// Робот добавляется как есть, со своими способностями; живой робот
	// должен стоять на свободной проходимой клетке, иначе runtime_error.
	void restore_robot(Robot robot, int x, int y, std::vector<std::shared_ptr<Robot_abilities>> abilities);
	// номер хода (>= 1), ранг, до которого награды уже выданы,
	// и применял ли игрок способность в этом ходу
	void restore_progress(int turn, int rewarded_rank, bool player_used_ability);
	void restore_inventory(const Inventory& inventory);
	// заменяет журнал; лишние старые записи отбрасываются
	void restore_log(const std::vector<std::string>& entries);

	// сложность и её модификаторы (задаются при создании уровня)
	void set_difficulty(const Difficulty& difficulty);
	const Difficulty& get_difficulty() const;
	// модификатор Single_ability_per_turn: робот больше не может
	// применять способности в этом ходу
	bool is_ability_limit_reached(int index) const;
	bool has_player_used_ability() const;

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
	const Robot_roster& get_robots() const;
	const Factory_roster& get_factories() const;
	bool is_inside_board(int x, int y) const;
	// внутри поля, не препятствие и не завод
	bool is_passable(int x, int y) const;
	// проходима и не занята живым роботом
	bool is_cell_free(int x, int y) const;

	// Способности роботов.
	void add_ability(int index, std::shared_ptr<Robot_abilities> ability);
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

	// Награды за ранг (см. Rank_rewards).
	bool has_rank_reward() const;
	int get_rewarded_rank() const;
	bool learn_ability(std::shared_ptr<Robot_abilities> ability);
	bool upgrade_ability(int ability_index);

	// Инвентарь игрока.
	bool add_item(std::shared_ptr<Item> item);
	bool use_item(int slot);
	const Inventory& get_inventory() const;

	// копия состояния игрока для переноса на следующий уровень;
	// способности передаются теми же объектами, чтобы сохранить улучшения
	Player_state get_player_state() const;
	// заменяет робота игрока, его способности и инвентарь на перенесённые
	// с прошлого уровня; координаты игрока не меняются.
	// Бросает runtime_error, если игрок ещё не добавлен.
	void set_player_state(const Player_state& state);

	// Сообщения о событиях (см. Event_reporter).
	void log_event(const std::string& text);
	void log_error(const std::string& text);
	void trace_event(const std::string& text) const;
	// слушатель событий (не владеет им); nullptr — без слушателя
	void set_event_listener(Game_event_listener* listener);
	const Event_log& get_log() const;
};

#endif
