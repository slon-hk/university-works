#ifndef OOP_PROJECT_RELATION_H
#define OOP_PROJECT_RELATION_H

// Отношение робота к игроку: враг, нейтральный (сам игрок) или союзник
enum class Relation {
	Enemy = -1,
	Neutral = 0,
	Ally = 1
};

#endif
