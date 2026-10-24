#include "includes/Status_list.h"

using namespace std;

Status_list::Status_list() {
}

Status_list::Status_list(const Status_list& other) {
	copy_from(other);
}

Status_list& Status_list::operator=(const Status_list& other) {
	if (this != &other) {
		copy_from(other);
	}
	return *this;
}

void Status_list::copy_from(const Status_list& other) {
	statuses_.clear();
	for (size_t i = 0; i < other.statuses_.size(); i++) {
		// клонируем, а не копируем указатели — иначе копии робота делили бы статусы
		statuses_.push_back(other.statuses_[i]->clone());
	}
}

void Status_list::add(shared_ptr<Robot_status> status) {
	if (status == nullptr) {
		return;
	}
	for (size_t i = 0; i < statuses_.size(); i++) {
		// если такой статус уже есть — объединяем вместо добавления дубликата
		if (statuses_[i]->merge(*status)) {
			return;
		}
	}
	statuses_.push_back(status);
}

void Status_list::clear_statuses() {
	statuses_.clear();
}

bool Status_list::blocks_movement() const {
	for (size_t i = 0; i < statuses_.size(); i++) {
		if (statuses_[i]->blocks_movement()) {
			return true;
		}
	}
	return false;
}

bool Status_list::try_activate_ability() {
	bool blocked = false;
	for (size_t i = 0; i < statuses_.size(); i++) {
		if (statuses_[i]->blocks_ability()) {
			blocked = true;
		}
		// уведомляем все статусы, даже если блокировка уже найдена,
		// чтобы каждый Overload «израсходовался»
		statuses_[i]->on_ability_attempt();
	}
	remove_expired();
	return !blocked;
}

int Status_list::absorb_damage(int damage) {
	for (size_t i = 0; i < statuses_.size(); i++) {
		// урон последовательно проходит через все статусы
		damage = statuses_[i]->absorb_damage(damage);
	}
	remove_expired();
	return damage;
}

int Status_list::process_turn_end() {
	int damage = 0;
	for (size_t i = 0; i < statuses_.size(); i++) {
		damage += statuses_[i]->on_turn_end();
	}
	remove_expired();
	return damage;
}

int Status_list::get_attack_bonus() const {
	int bonus = 0;
	for (size_t i = 0; i < statuses_.size(); i++) {
		bonus += statuses_[i]->get_attack_bonus();
	}
	return bonus;
}

int Status_list::get_count() const {
	return (int) statuses_.size();
}

const Robot_status& Status_list::get(int index) const {
	return *statuses_.at(index);
}

void Status_list::remove_expired() {
	// i увеличиваем только если элемент не удалён
	for (size_t i = 0; i < statuses_.size();) {
		if (statuses_[i]->is_expired()) {
			statuses_.erase(statuses_.begin() + i);
		} else {
			i++;
		}
	}
}
