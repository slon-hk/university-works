#ifndef OOP_PROJECT_OVERLOAD_H
#define OOP_PROJECT_OVERLOAD_H

#include "Base_status.h"

// Перегрузка: блокирует следующую попытку использовать способность,
// после чего снимается. Повторные наложения не складываются.
class Overload : public Base_status {
private:
	bool expired_;

public:
	Overload();

	std::string get_name() const override;
	std::string get_description() const override;
	int get_amount() const override;
	std::shared_ptr<Robot_status> clone() const override;
	bool merge(const Robot_status& other) override;
	bool blocks_ability() const override;
	void on_ability_attempt() override;

	bool is_expired() const override;
};

#endif
