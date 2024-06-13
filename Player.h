#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"
#include "MyWindow.h"
#include "DestroyAnimation.h"
#include "Enums.h"

using namespace std;
using namespace sf;

class Player {
public:
	Player();
	~Player() {};

	void render(MyWindow* w);
	int getLives();
	int getPoints();
	void move(float distance);
	int isPlayerHit(Vector2f position);
	float getSpeed();
	Vector2f getLaserPosition();
	void updateHit(int pts);
	void setDirection(Direction dir);
	int showDestroy();
	void hideDestroy();
	void restart();

private:
	int lives;
	int points;
	Texture laserTexture;
	Sprite laser;
	vector<Sprite> livesLeft;
	DestroyAnimation destroyer;
	int show;
	float speed;
	Direction direction;

	void initializeComponent(Sprite &s, Vector2f pos);
};
#endif // !PLAYER_H