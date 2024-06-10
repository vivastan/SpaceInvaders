#ifndef ALIENS_H
#define ALIENS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "DestroyAnimation.h"
#include "Bullet.h"
#include "Defense.h"

using namespace std;
using namespace sf;

#define LEFT -1 // enum
#define RIGHT 1

class Player;

class Aliens {
public:
	Aliens();
	~Aliens();

	float getSpeed();
	void render(MyWindow* w, Time time);
	float getBulletSpeed();
	void update(Player* p, Defense* d);
	void move(Defense* d);
	void startShooting();
	int isAlienHit(Vector2f position);
	int isShooting();
	float getLowestAlienPosition();
	Vector2f getAlienPosition();

private:
	Texture alien1Txt;
	Texture alien2Txt;
	Texture alien3Txt;
	vector<vector<int>> states; // 0 - nije pogoden, 1 - pogoden je
	vector<vector<Sprite>> aliens;
	int direction;
	float speed;
	DestroyAnimation destroyer;
	int show;
	Bullet bullet;

	int onHit(int x, int y) {
		states[x][y] = 1;
		destroyer.destroyed(aliens[x][y].getPosition());
		aliens[x][y].setScale(0, 0);
		show = 1;
		if (x == 0) return 3;
		else if (x < 3) return 2;
		return 1;
	}

	int getLeftPosition() {
		for (int j = 0; j < 11; j++) {
			for (int i = 0; i < 5; i++) {
				if (!states[i][j]) {
					return aliens[i][j].getPosition().x;
				}
			}
		}
		return 0; // ovo mi ne bi trebalo trebati uopce - napravi provjeru u fji
	}

	int getRightPosition() {
		for (int j = 10; j >= 0; j--) {
			for (int i = 0; i < 5; i++) {
				if (!states[i][j]) {
					return aliens[i][j].getPosition().x;
				}
			}
		}
		return 0; // ovo mi ne bi trebalo trebati uopce - napravi provjeru u fji
	}
};
#endif // !ALIENS_H