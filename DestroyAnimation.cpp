#include "DestroyAnimation.h"

DestroyAnimation::DestroyAnimation(Color color) {
	show = 0;
	destroyAnimation.resize(8);
	for (int i = 0; i < 8; i++) {
		destroyAnimation[i].setFillColor(color);
		destroyAnimation[i].setSize(Vector2f(1, 12.5));
		destroyAnimation[i].setOrigin(0.5, 12.5);
		destroyAnimation[i].setRotation(i * 45);
	}

	destroyCircle.setRadius(7.5);
	destroyCircle.setOrigin(7.5, 7.5);
	destroyCircle.setFillColor(Color::Black);
}

DestroyAnimation::~DestroyAnimation() {}

void DestroyAnimation::destroyed(Vector2f position) {
	for (int i = 0; i < 8; i++)
		destroyAnimation[i].setPosition(position);
	destroyCircle.setPosition(position);
	show = 1;
}

void DestroyAnimation::render(MyWindow* w) {
	for (int i = 0; i < 8; i++)
		w->draw(destroyAnimation[i]);
	w->draw(destroyCircle);
}

int DestroyAnimation::isShowing() {
	return show;
}

void DestroyAnimation::hide() {
	show = 0;
}