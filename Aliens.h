#ifndef ALIENS_H
#define ALIENS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "DestroyAnimation.h"
#include "Defense.h"
#include "Enums.h"

using namespace std;
using namespace sf;

class Aliens {
public:
	Aliens();
	~Aliens();

	float getSpeed();
	void render(MyWindow* w);
	// void update(Player* p, Defense* d);
	void move(Defense* d);
	int isAlienHit(Vector2f position);
	float getLowestAlienPosition();
	Vector2f getAlienPosition();
	int allDestroyed();
	int showDestroy();
	void hideDestroy();
	void restart(int _startSpeed, int _startY);

private:
	Texture alien1Txt;
	Texture alien2Txt;
	Texture alien3Txt;
	vector<vector<int>> states; // 0 - nije pogoden, 1 - pogoden je
	vector<vector<Sprite>> aliens;
	Direction direction;
	int startY;
	float startSpeed;
	float speed;
	DestroyAnimation destroyer;
	int show;
	// Bullet bullet;

	int onHit(int x, int y);
	int getLeftPosition();
	int getRightPosition();
};
#endif // !ALIENS_H