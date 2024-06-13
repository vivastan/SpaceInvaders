#ifndef ALIENS_H
#define ALIENS_H

#include <SFML/Graphics.hpp>
#include "MyWindow.h"
#include "Defense.h"
#include "DestroyAnimation.h"
#include "Enums.h"

using namespace std;
using namespace sf;

class Aliens {
public:
	Aliens();
	~Aliens();

	float getSpeed();
	void render(MyWindow* w);
	void move(Defense* d);
	int isAlienHit(Vector2f position);
	float getLowestAlienPosition();
	Vector2f getAlienPosition();
	int allDestroyed();
	int isDestroyShowing();
	void hideDestroy();
	void restart(int _startSpeed, int _startY);

private:
	Texture alien1Txt;
	Texture alien2Txt;
	Texture alien3Txt;
	vector<vector<Sprite>> aliens;
	Direction direction;
	int startY;
	float startSpeed;
	float speed;
	DestroyAnimation destroyer;

	int isHit(int x, int y);
	int onHit(int x, int y);
	int getLeftPosition();
	int getRightPosition();
};
#endif // !ALIENS_H