#ifndef DESTROY_ANIMATION_H
#define DESTROY_ANIMATION_H

#include "SFML/Graphics.hpp"
#include "MyWindow.h"

using namespace sf;
using namespace std;

class DestroyAnimation {
public:
	DestroyAnimation() {
		destroyAnimation.resize(8);
		for (int i = 0; i < 8; i++) {
			destroyAnimation[i].setFillColor(Color::Green);
			destroyAnimation[i].setSize(Vector2f(1, 12.5));
			destroyAnimation[i].setOrigin(0.5, 12.5);
			destroyAnimation[i].setRotation(i * 45);
		}

		destroyCircle.setRadius(7.5);
		destroyCircle.setOrigin(7.5, 7.5);
		destroyCircle.setFillColor(Color::Black);
	}

	void destroyed(Vector2f position) {
		for (int i = 0; i < 8; i++)
			destroyAnimation[i].setPosition(position);
		destroyCircle.setPosition(position);
	}

	void render(MyWindow *w) {
		for (int i = 0; i < 8; i++)
			w->draw(destroyAnimation[i]);
		w->draw(destroyCircle);
	}

private:
	vector<RectangleShape> destroyAnimation;
	CircleShape destroyCircle;
};
#endif // !DESTROY_ANIMATION_H