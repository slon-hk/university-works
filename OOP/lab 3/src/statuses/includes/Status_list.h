#ifndef OOP_PROJECT_STATUS_LIST_H
#define OOP_PROJECT_STATUS_LIST_H

#include <memory>
#include <vector>

#include "Robot_status.h"

// Список статусов робота. Владеет своими статусами: при копировании робота
// статусы клонируются, чтобы копии не делили одни и те же объекты.
// Отвечает за слияние однотипных статусов и удаление истёкших.
class Status_list {
private:
	std::vector<std::shared_ptr<Robot_status>> statuses_;

	void copy_from(const Status_list& other);
	void remove_expired();

public:
	Status_list();
	Status_list(const Status_list& other);
	Status_list& operator=(const Status_list& other);

	// добавляет статус или сливает его с уже имеющимся того же типа
	void add(std::shared_ptr<Robot_status> status);
	// удаляет все статусы
	void clear_statuses();

	bool blocks_movement() const;

	// сообщает всем статусам о попытке применить способность;
	// false, если хотя бы один её заблокировал
	bool try_activate_ability();

	// пропускает урон через все статусы, возвращает оставшийся урон
	int absorb_damage(int damage);

	// обрабатывает конец хода, возвращает суммарный урон от статусов
	int process_turn_end();
	// суммарный бонус к атаке
	int get_attack_bonus() const;

	int get_count() const;
	const Robot_status& get(int index) const;
};

#endif
