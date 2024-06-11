#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"
#include "MyWindow.h"
#include "DestroyAnimation.h"
#include "Enums.h"

#include <iostream>

using namespace std;
using namespace sf;

class Player {
public:
	Player();
	~Player() {};

	// void update(Aliens* a, Defense *d);
	void render(MyWindow* w);
	int getLives();
	void move();
	// void startShooting();
	int isPlayerHit(Vector2f position);
	float getSpeed();
	Vector2f getLaserPosition();
	void updateHit(int pts);
	void setDirection(Direction dir);
	int showDestroy();
	void hideDestroy();

private:
	int lives;
	int points;
	Texture laserTexture;
	Sprite laser;
	// Bullet bullet;
	vector<Sprite> livesLeft;
	Font font;
	Text textPoints;
	DestroyAnimation destroyer;
	int show;
	float speed;
	Direction direction;

	void updatePoints();
	void setText(Text* text, int x, int y);
	// int isShooting();
	// void stopShooting();
};
#endif // !PLAYER_H