#ifndef DEFENSE_H
#define DEFENSE_H

#include "SFML/Graphics.hpp"
#include "MyWindow.h"
#include "Enums.h"

using namespace sf;
using namespace std;

class Defense {
public:
	Defense();
	~Defense();

	void render(MyWindow* w);
	int isHit(Vector2f position, Object obj);
	void onCollision(Vector2f pos);
	void restart();

private:
	Texture defenseTexture;
	vector<Sprite> defense;
	vector<vector<RectangleShape>> defenseDestroyed;

	void initializeDefense(int i);
	void initializeDefenseDestroyers(int i, float width, float height);
	void onDestroy(int i, int j);
	int isDestroyed(int i, int j);
};
#endif // !DEFENSE_H