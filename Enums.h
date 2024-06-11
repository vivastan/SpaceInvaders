#ifndef ENUMS_H
#define ENUMS_H

#define ALIEN_COLOR 207, 107, 170
#define PLAYER_COLOR 198, 199, 191

enum class GameFlow {
	menu,
	over,
	on,
	restart,
	playerHit
};

enum class Direction {
	left = -1,
	none,
	right
};

enum class Object {
	alien,
	player
};

#endif // ENUMS_H