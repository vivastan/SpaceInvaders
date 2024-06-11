#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include "MyWindow.h"
#include "Defense.h"
#include "Aliens.h"
#include "Player.h"

using namespace sf;

class Bullet {
public:
	Bullet(Object obj);
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
	Object object;
};
#endif // !BULLET_H