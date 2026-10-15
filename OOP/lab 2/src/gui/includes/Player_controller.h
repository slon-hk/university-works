#ifndef OOP_PROJECT_PLAYER_CONTROLLER_H
#define OOP_PROJECT_PLAYER_CONTROLLER_H

#include "../../abilities/includes/Ability_catalog.h"
#include "../../board/includes/Direction.h"
#include "../../game/includes/Game_session.h"

// Переводит команды игрока (из Gui) в действия над Game_session и хранит
// состояние ввода: выбранный слот способности и режим выбора цели.
// При невозможности дальнейших действий автоматически завершает ход.
class Player_controller {
private:
	Game_session& session_;
	Ability_catalog catalog_;
	int selected_slot_; // слот на панели способностей (индекс в каталоге)
	bool targeting_; // ждём клика по целевой клетке
	bool ability_switched_; // способность уже переключали в этом ходу
	bool level_complete_reported_; // сообщение о прохождении уровня уже выведено

	// остались ли у игрока очки движения или энергия на какую-нибудь способность
	bool can_act() const;
	void report_level_complete();
	// вызывается после каждого действия игрока
	void after_action();

public:
	explicit Player_controller(Game_session& session);

	const Ability_catalog& get_catalog() const;
	int get_selected_slot() const;
	// индекс выбранной способности у игрока или -1, если она не изучена
	int get_selected_ability_index() const;
	bool is_targeting() const;
	bool is_ability_switched() const;

	// выбор стартовой способности (только пока у игрока нет ни одной)
	void choose_start_ability(int slot);
	// переключение способности — не чаще раза за ход
	void select_ability(int slot);

	// применяет выбранную способность или включает выбор цели
	void activate_selected_ability();
	void use_ability_at(int x, int y);
	void cancel_targeting();
	void move(Direction direction);
	void use_item(int slot);
	// изучить способность из слота или улучшить её, если уже изучена
	void choose_rank_reward(int slot);
	void end_turn();
};

#endif
