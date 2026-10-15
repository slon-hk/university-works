#ifndef OOP_PROJECT_ROBOT_FACTORY_H
#define OOP_PROJECT_ROBOT_FACTORY_H

#include <cstddef>
#include <vector>

#include "Robot.h"

// Завод роботов: раз в spawn_period ходов выпускает factory_output копий
// шаблонного робота unit_, усиленных в зависимости от уровня завода.
// Занимает на поле квадрат get_size() x get_size() клеток. У завода есть
// здоровье: разрушенный завод больше не производит роботов.
class Robot_factory {
private:
	Robot unit_; // шаблон производимого робота
	std::size_t factory_output_; // сколько роботов выпускается за раз
	std::size_t level_; // уровень завода (>= 1), даёт бонус к характеристикам
	std::size_t spawn_period_; // период производства в ходах (>= 1)
	std::size_t turns_until_spawn_; // ходов до следующего выпуска
	std::size_t current_health_;
	std::size_t max_health_;

public:
	static const std::size_t kDefaultHealth = 100;

	Robot_factory(const Robot& unit, std::size_t factory_output, std::size_t level, std::size_t spawn_period,
				  std::size_t max_health = kDefaultHealth);

	void set_level(std::size_t level);
	void set_unit(const Robot& unit);
	void set_factory_output(std::size_t factory_output);
	void set_spawn_period(std::size_t spawn_period);

	std::size_t get_level() const;
	Robot get_unit() const;
	std::size_t get_factory_output() const;
	std::size_t get_spawn_period() const;
	std::size_t get_turns_until_spawn() const;
	// размер стороны завода в клетках
	int get_size() const;
	std::size_t get_current_health() const;
	std::size_t get_max_health() const;

	// снижает здоровье на damage (не ниже нуля)
	void take_damage(int damage);
	bool is_destroyed() const;

	// создаёт одного робота с бонусами за уровень завода
	Robot create() const;
	// вызывается раз в ход: уменьшает счётчик и, когда он дошёл до нуля,
	// возвращает новую партию роботов (иначе — пустой вектор);
	// разрушенный завод всегда возвращает пустой вектор
	std::vector<Robot> produce();
};

#endif
