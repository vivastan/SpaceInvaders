#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include "MyWindow.h"
#include "Defense.h"

using namespace sf;

#define UP 1
#define DOWN 2 // enum

class Aliens;
class Player;

class Bullet {
public:
	Bullet(Color color);
	float getSpeed();
	void render(MyWindow* w);
	void hide();
	void show(Vector2f position);
	int isShowing();
	Vector2f getPosition();
	void shoot(Defense* d, Aliens* a, Player* p);
	void startShooting(Aliens* a, Player* p);

private:
	RectangleShape bullet;
	float speed;
	int direction;
};
#endif // !BULLET_H