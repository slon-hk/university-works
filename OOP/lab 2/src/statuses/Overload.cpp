#include "includes/Overload.h"

using namespace std;

Overload::Overload() : expired_(false) {
}

string Overload::get_name() const {
	return "Overload";
}

string Overload::get_description() const {
	return "Overload";
}

int Overload::get_amount() const {
	return 1;
}

shared_ptr<Robot_status> Overload::clone() const {
	return make_shared<Overload>(*this);
}

bool Overload::merge(const Robot_status& other) {
	// второй Overload просто поглощается — эффект не усиливается
	return other.get_name() == get_name();
}

bool Overload::blocks_ability() const {
	return true;
}

void Overload::on_ability_attempt() {
	// одна заблокированная попытка — и перегрузка снимается
	expired_ = true;
}

bool Overload::is_expired() const {
	return expired_;
}
