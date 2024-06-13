#include "Defense.h"

Defense::Defense() {
	defenseTexture.loadFromFile("defense.png");

	defense.resize(4);
	defenseDestroyed.resize(4, vector<RectangleShape>(12)); /* za 4 obrambena objekta svaki se unistava s 12 metaka */
	for (int i = 0; i < 4; i++) {
		initializeDefense(i);
		initializeDefenseDestroyers(i, defense[i].getLocalBounds().width * 0.2, defense[i].getLocalBounds().height * 0.2);
	}
}

Defense::~Defense() {}

void Defense::render(MyWindow* w) {
	for (int i = 0; i < 4; i++) {
		w->draw(defense[i]);
		for (int j = 0; j < 12; j++) {
			w->draw(defenseDestroyed[i][j]);
		}
	}
}

/* provjera je li obrambeni objekt pogoden */
int Defense::isHit(Vector2f position, Object obj) {
	float x = position.x, y = position.y, width = defense[0].getLocalBounds().width * 0.2, height = defense[0].getLocalBounds().height * 0.2;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 12; j++) {
			if (isDestroyed(i, j)) continue; /* taj dio obrane vec je unisten */

			float dX = defenseDestroyed[i][j].getPosition().x, dY = defenseDestroyed[i][j].getPosition().y;

			if (dX <= x && x <= dX + width / 3 && dY <= y && y <= dY + height / 4) {
				/* provjera je li dio iznad ovog nadenog vec pogoden - ako nije, prvo njega unisti */
				if (obj == Object::alien && j >= 3 && !isDestroyed(i, j - 3)) {
					onDestroy(i, j - 3);
				}
				/* provjera je li dio ispod ovog nadenog vec pogoden - ako nije, prvo njega unisti */
				else if (obj == Object::player && j <= 8 && !isDestroyed(i, j + 3)) {
					onDestroy(i, j + 3);
				}
				else {
					onDestroy(i, j);
				}
				return 1;
			}
		}
	}
	return 0;
}

/* provjera je li se vanzemaljac zabio u obrambeni objekt */
void Defense::onCollision(Vector2f pos) {
	float x = pos.x, y = pos.y,
		width = defense[0].getLocalBounds().width * 0.2, height = defense[0].getPosition().y - defense[0].getLocalBounds().height * 0.2 / 2;

	for (int i = 0; i < 4; i++) {
		float left = defense[i].getPosition().x - width / 2, right = defense[i].getPosition().x + width / 2;
		if (left <= x && x <= right && height <= y) { /* ako se zabio */
			for (int j = 0; j < 12; j++) { /* unisti cijelu obranu */
				onDestroy(i, j);
			}
			return;
		}
	}
	return;
}

void Defense::restart() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 12; j++) {
			if (j == 10) { /* ovaj ne pokriva objekt (sliku) uopce pa ga odmah inicijaliziram kao da je vec pogoden */
				onDestroy(i, j);
			}
			else {
				defenseDestroyed[i][j].setScale(0, 0);
			}
		}
	}
}

void Defense::initializeDefense(int i) {
	defense[i].setTexture(defenseTexture);
	defense[i].setOrigin(defense[i].getLocalBounds().width / 2, defense[i].getLocalBounds().height / 2);
	defense[i].setPosition(Vector2f(100 + i * 200, 600));
	defense[i].setScale(Vector2f(0.2f, 0.2f));
}

void Defense::initializeDefenseDestroyers(int i, float width, float height) {
	for (int j = 0; j < 12; j++) {
		defenseDestroyed[i][j].setSize(Vector2f(width / 3, height / 4));
		defenseDestroyed[i][j].setFillColor(Color::Black);
		defenseDestroyed[i][j].setPosition(Vector2f(100 - width / 2 + (j % 3) * width / 3 + i * 200, 600 - height / 2 + j / 3 * height / 4));
		if (j == 10) { /* ovaj ne pokriva objekt (sliku) uopce pa ga odmah inicijaliziram kao da je vec pogoden */
			onDestroy(i, j);
		}
		else {
			defenseDestroyed[i][j].setScale(0, 0);
		}
	}
}

/* kad je dio obrane unisten, prikazi pravokutnik koji ga pokriva */
void Defense::onDestroy(int i, int j) {
	defenseDestroyed[i][j].setScale(1, 1);
}

/* vraca 1 ako je taj dio obrane vec unisten (tj ako je pravokutnik postavljen), inace 0 */
int Defense::isDestroyed(int i, int j) {
	return (defenseDestroyed[i][j].getScale() == Vector2f(0, 0)) ? 0 : 1;
}