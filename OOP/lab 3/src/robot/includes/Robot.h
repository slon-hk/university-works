#ifndef OOP_PROJECT_ROBOT_H
#define OOP_PROJECT_ROBOT_H

#include <cstddef>
#include <memory>

#include "Relation.h"
#include "../../statuses/includes/Robot_status.h"
#include "../../statuses/includes/Status_list.h"

// Робот — основная боевая единица: хранит характеристики (здоровье, энергия,
// атака, опыт, ранг, обзор, скорость), отношение к игроку и наложенные статусы.
// Все характеристики ограничены сверху константой kMaxStat.
class Robot {
public:
	static constexpr std::size_t kMaxStat = 100000;

private:
	std::size_t current_health_;
	std::size_t max_health_;
	std::size_t attack_power_;
	std::size_t current_energy_;
	std::size_t max_energy_;
	std::size_t current_experience_;
	std::size_t experience_to_next_rank_;
	std::size_t rank_;
	std::size_t vision_radius_; // радиус обзора в клетках
	std::size_t heal_power_; // сколько HP восстанавливает союзнику при взаимодействии
	std::size_t speed_; // очки движения, восстанавливаемые каждый ход
	std::size_t movement_points_; // оставшиеся очки движения в текущем ходу
	Relation relation_;
	Status_list statuses_; // активные эффекты (горение, щит и т.п.)

	// прибавляет delta к value и зажимает результат в [lower, upper]
	std::size_t clamp_add(std::size_t value, int delta, std::size_t lower, std::size_t upper) const;
	// повышает ранг, пока хватает опыта
	void rank_up_if_ready();

public:
	Robot(
		std::size_t current_health,
		std::size_t max_health,
		std::size_t attack_power,
		std::size_t current_energy,
		std::size_t max_energy,
		std::size_t current_experience,
		std::size_t experience_to_next_rank,
		std::size_t rank,
		std::size_t vision_radius,
		std::size_t heal_power,
		std::size_t speed,
		Relation relation
	);

	// Изменение характеристик на delta (может быть отрицательным),
	// результат всегда остаётся в допустимых границах.
	void change_health(int delta);
	void change_energy(int delta);
	void change_experience(int delta);
	void change_attack_power(int delta);
	void change_vision_radius(int delta);
	void change_rank(int delta);
	void change_experience_to_next_rank(int delta);
	void change_max_health(int delta);
	void change_max_energy(int delta);
	void change_heal_power(int delta);
	void change_speed(int delta);
	// восстанавливает энергию до максимума (при переходе на следующий уровень)
	void reset_energy();

	// Тратит cost очков движения; false, если очков не хватает.
	bool spend_movement_points(int cost);
	void restore_movement_points();
	// false, если статус (например, Slow) запрещает перемещение
	bool can_move() const;

	// Враги — это пара «Enemy» и «не Enemy».
	bool is_enemy_of(const Robot& other) const;
	// Взаимодействовать можно с врагом (атака) или с союзником (лечение).
	bool can_interact_with(const Robot& other) const;
	// Атакует врага или лечит союзника в зависимости от отношения.
	void interaction_with_robot(Robot* robot);
	// Наносит урон другому роботу; за убийство начисляется опыт.
	void deal_damage(Robot* robot, int damage);
	// Получает урон с учётом поглощения статусами (щит).
	void take_damage(int damage);

	// Работа со статусами.
	void add_status(std::shared_ptr<Robot_status> status);
	// снимает все статусы (при переходе на следующий уровень)
	void clear_statuses();
	// false, если способность заблокирована (Overload)
	bool try_activate_ability();
	// применяет эффекты конца хода (урон от горения и т.п.)
	void process_statuses_turn_end();
	int get_status_count() const;
	const Robot_status& get_status(int index) const;

	std::size_t get_current_health() const;
	std::size_t get_max_health() const;
	// атака с учётом бонусов от статусов
	std::size_t get_attack_power() const;
	// бонус к атаке от статусов (Strength)
	std::size_t get_attack_bonus() const;
	std::size_t get_current_energy() const;
	std::size_t get_max_energy() const;
	std::size_t get_current_experience() const;
	std::size_t get_experience_to_next_rank() const;
	std::size_t get_rank() const;
	std::size_t get_vision_radius() const;
	std::size_t get_heal_power() const;
	std::size_t get_speed() const;
	std::size_t get_movement_points() const;
	Relation get_relation() const;
	bool is_alive() const;
};

#endif
