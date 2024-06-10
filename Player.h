#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"
#include "MyWindow.h"
#include "DestroyAnimation.h"
#include "Bullet.h"

#include <iostream>

#define UP 1
#define DOWN 2 // enum

using namespace std;
using namespace sf;

class Aliens;

class Player {
public:
	Player();
	~Player() {};

	void update(Aliens* a, Defense *d);
	void render(MyWindow* w, Time time);
	int getLives();
	void move(int direction);
	void startShooting();
	int isPlayerHit(Vector2f position);
	float getSpeed();
	Vector2f getLaserPosition();

private:
	int lives;
	int points;
	Texture laserTexture;
	Sprite laser;
	Bullet bullet;
	vector<Sprite> livesLeft;
	Font font;
	Text textPoints;
	DestroyAnimation destroyer;
	int show;
	Time wait;
	float speed;

	void updatePoints();
	void updateHit(int pts);
	void setText(Text* text, int x, int y);
	int isShooting();
	void stopShooting();
};
#endif // !PLAYER_H