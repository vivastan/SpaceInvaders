#ifndef DESTROY_ANIMATION_H
#define DESTROY_ANIMATION_H

#include "SFML/Graphics.hpp"
#include "MyWindow.h"

using namespace sf;
using namespace std;

class DestroyAnimation {
public:
	DestroyAnimation(Color color);
	~DestroyAnimation();

	void destroyed(Vector2f position);
	void render(MyWindow* w);
	int isShowing();
	void hide();

private:
	vector<RectangleShape> destroyAnimation;
	CircleShape destroyCircle;
	int show;
};
#endif // !DESTROY_ANIMATION_H