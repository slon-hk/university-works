#ifndef OOP_PROJECT_PLAYER_CONTROLLER_H
#define OOP_PROJECT_PLAYER_CONTROLLER_H

#include "../../abilities/includes/Ability_catalog.h"
#include "../../board/includes/Direction.h"
#include "Game_session.h"

// Переводит команды игрока в действия над Game_session и хранит
// состояние ввода: выбранный слот способности и режим выбора цели
// (цель выбирается курсором, который двигается командами перемещения).
// При невозможности дальнейших действий автоматически завершает ход.
class Player_controller {
private:
	Game_session& session_;
	Ability_catalog catalog_;
	int selected_slot_; // слот на панели способностей (индекс в каталоге)
	bool targeting_; // идёт выбор целевой клетки
	int cursor_x_; // клетка под курсором прицела
	int cursor_y_;
	bool ability_switched_; // способность уже переключали в этом ходу
	bool level_complete_reported_; // сообщение о прохождении уровня уже выведено

	// сдвигает курсор прицела, не выводя его за поле и радиус способности
	void move_cursor(Direction direction);
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
	int get_cursor_x() const;
	int get_cursor_y() const;
	bool is_ability_switched() const;

	// восстановление из сохранения: выбранный слот и было ли переключение
	// в этом ходу; выбор цели сбрасывается
	void restore(int selected_slot, bool ability_switched);
	// выбор стартовой способности (только пока у игрока нет ни одной)
	void choose_start_ability(int slot);
	// переключение способности — не чаще раза за ход
	void select_ability(int slot);

	// применяет выбранную способность или включает выбор цели;
	// в режиме выбора цели — применяет её к клетке под курсором
	void activate_selected_ability();
	void use_ability_at(int x, int y);
	void cancel_targeting();
	// шаг игрока, а в режиме выбора цели — сдвиг курсора
	void move(Direction direction);
	void use_item(int slot);
	// изучить способность из слота или улучшить её, если уже изучена
	void choose_rank_reward(int slot);
	void end_turn();
};

#endif
