#ifndef OOP_PROJECT_ROBOT_STATUS_H
#define OOP_PROJECT_ROBOT_STATUS_H

#include <memory>
#include <string>

class Game_session;

// Интерфейс статуса (эффекта), который может висеть на роботе или лежать
// на клетке поля. Набор хуков позволяет статусу влиять на движение, способности,
// получаемый урон, атаку и действовать в конце хода.
class Robot_status {
public:
	virtual ~Robot_status() = default;

	// имя типа статуса — по нему сравниваются статусы при слиянии
	virtual std::string get_name() const = 0;
	// текст для лога и интерфейса, например "Burn(3)"
	virtual std::string get_description() const = 0;
	// числовая величина статуса (ходы, очки щита и т.п.)
	virtual int get_amount() const = 0;
	// глубокая копия — нужна, чтобы у каждого робота был свой экземпляр
	virtual std::shared_ptr<Robot_status> clone() const = 0;

	// пытается объединить с однотипным статусом; true — объединили,
	// и other добавлять отдельно не нужно
	virtual bool merge(const Robot_status& other) = 0;

	// запрещает ли статус перемещение / использование способностей
	virtual bool blocks_movement() const = 0;
	virtual bool blocks_ability() const = 0;

	// вызывается при каждой попытке применить способность
	virtual void on_ability_attempt() = 0;

	// получает входящий урон и возвращает, сколько осталось после поглощения
	virtual int absorb_damage(int damage) = 0;

	// вызывается в конце хода, возвращает урон, который надо нанести роботу
	virtual int on_turn_end() = 0;

	// вызывается в момент наложения на робота с индексом robot_index
	virtual void on_apply(Game_session& session, int robot_index) = 0;

	// прибавка к атаке робота
	virtual int get_attack_bonus() const = 0;

	// true — статус закончился и должен быть удалён
	virtual bool is_expired() const = 0;
};

#endif //OOP_PROJECT_ROBOT_STATUS_H
