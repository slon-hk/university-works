#ifndef OOP_PROJECT_ENEMY_CONTROLLER_H
#define OOP_PROJECT_ENEMY_CONTROLLER_H

class Game_session;


// Простой ИИ для всех роботов, кроме игрока. Враг, видящий игрока,
// идёт к нему (и атакует, упираясь в него), иначе бродит случайно.
// Союзники только бродят.
class Enemy_controller {
private:
	// враг ли это и находится ли игрок в его радиусе обзора
	bool sees_player(const Game_session& session, int index) const;
	// лечится, пока есть энергия и HP не полное; телепортируется к игроку
	void use_abilities(Game_session& session, int index) const;
	// телепорт на соседнюю с игроком клетку
	void teleport_to_player(Game_session& session, int index, int ability_index) const;
	// перемещение/атака робота за ход
	void act(Game_session& session, int index) const;
	// шаг в сторону игрока по большей из осей; false, если шагнуть не удалось
	bool step_towards_player(Game_session& session, int index) const;
	bool step_randomly(Game_session& session, int index) const;

public:
	// выполняет ход всех неигровых роботов
	void run_turn(Game_session& session) const;
};

#endif
