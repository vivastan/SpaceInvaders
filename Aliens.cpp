#include "Aliens.h"

#define ALIEN_COLOR 207, 107, 170

Aliens::Aliens() : destroyer(Color(ALIEN_COLOR)) /*, bullet(Color::White) */ {
	direction = Direction::left;
	show = 0;

	startY = 50;
	startSpeed = 2.8;

	alien1Txt.loadFromFile("alien1.png");
	alien2Txt.loadFromFile("alien2.png");
	alien3Txt.loadFromFile("alien3.png");

	states.resize(5, vector<int>(11, 0));
	aliens.resize(5, vector<Sprite>(11));

	nextLevel();

	/*
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (i == 0) {
				aliens[i][j].setTexture(alien3Txt);
			}
			else if (i < 3) {
				aliens[i][j].setTexture(alien2Txt);
			}
			else {
				aliens[i][j].setTexture(alien1Txt);
			}
			aliens[i][j].setScale(Vector2f(0.15, 0.15));
			aliens[i][j].setOrigin(aliens[i][j].getLocalBounds().width / 2, aliens[i][j].getLocalBounds().height / 2);
			aliens[i][j].setPosition(Vector2f(75 + j * 65, 100 + i * 60));
			// cerr << aliens[i][j].getPosition().x << ", " << aliens[i][j].getPosition().y << endl;
			// cerr << aliens[i][j].getLocalBounds().width*0.15/2 << ", " << aliens[i][j].getLocalBounds().height * 0.15 / 2 << endl;
		}
	}
	*/
}

Aliens::~Aliens() {}

int Aliens::showDestroy() {
	return show;
}

void Aliens::hideDestroy() {
	show = 0;
}

float Aliens::getSpeed() {
	return speed;
}

void Aliens::render(MyWindow* w) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			w->draw(aliens[i][j]);
		}
	}
	// bullet.render(w);

	if (show) {
		destroyer.render(w);
	}
}

/*
void Aliens::update(Player* p, Defense* d) {
	if (!isShooting()) {
		startShooting();
	}
	else {
		bullet.shoot(d, NULL, p);
	}
} */

void Aliens::move(Defense* d) {
	Vector2f offset((direction == Direction::left) ? -speed : speed, 0);

	if (getLeftPosition() <= 25) {
		direction = Direction::right;
		offset.y += 2;
		speed += 0.1;
	}
	else if (getRightPosition() >= 775) {
		direction = Direction::left;
		offset.y += 2;
		speed += 0.1;
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			aliens[i][j].move(offset);
			if (states[i][j]) continue;

			float x = aliens[i][j].getPosition().x, y = aliens[i][j].getPosition().y;
			float width = d->getDefense(0).getLocalBounds().width * 0.2, height = d->getDefense(0).getPosition().y - d->getDefense(0).getLocalBounds().height * 0.2;
			for (int k = 0; k < 4; k++) {
				float defXL = d->getDefense(k).getPosition().x - width / 2, defXR = d->getDefense(k).getPosition().x + width / 2;
				if (defXL <= x && x <= defXR && height <= y) {
					cerr << "defense " << k << " destroyed by alien " << i << ", " << j << endl;
					while (d->onHit(k, Object::alien));
					break;
				}
			}
		}
	}

	// show = 0;
}

Vector2f Aliens::getAlienPosition() {
	vector<pair<int, int>> v;

	// da samo najnizi mogu pucati + na random
	for (int j = 0; j < 11; j++) {
		int i = 4;
		for (; i >= 0; i--) {
			if (!states[i][j]) {
				v.push_back(pair<int, int>(i, j));
				break;
			}
		}
	}

	if (v.size() == 0) return Vector2f(-1.f, -1.f);

	int t = rand() % v.size();

	return aliens[v[t].first][v[t].second].getPosition();
}

int Aliens::isAlienHit(Vector2f position) {
	float x = position.x, y = position.y;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (states[i][j]) continue;

			float downY = aliens[i][j].getPosition().y - aliens[i][j].getLocalBounds().height * 0.15 / 2,
				upY = aliens[i][j].getPosition().y + aliens[i][j].getLocalBounds().height * 0.15 / 2,
				leftX = aliens[i][j].getPosition().x - aliens[i][j].getLocalBounds().width * 0.15 / 2,
				rightX = aliens[i][j].getPosition().x + aliens[i][j].getLocalBounds().width * 0.15 / 2;

			if (downY <= y &&  y <= upY && leftX <= x && x <= rightX ) {
				cerr << i << ", " << j << endl;
				return onHit(i, j);
			}
		}
	}
	return 0;
}

float Aliens::getLowestAlienPosition() {
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 11; j++) {
			if (!states[i][j]) {
				return aliens[i][j].getPosition().y;;
			}
		}
	}
	return 0; // do ovog ne bi trebalo doci
}

int Aliens::onHit(int x, int y) {
	states[x][y] = 1;
	destroyer.destroyed(aliens[x][y].getPosition());
	aliens[x][y].setScale(0, 0);
	show = 1;
	if (x == 0) return 3;
	else if (x < 3) return 2;
	return 1;
}

int Aliens::getLeftPosition() {
	for (int j = 0; j < 11; j++) {
		for (int i = 0; i < 5; i++) {
			if (!states[i][j]) {
				return aliens[i][j].getPosition().x;
			}
		}
	}
	return 0; // ovo mi ne bi trebalo trebati uopce - napravi provjeru u fji
}

int Aliens::getRightPosition() {
	for (int j = 10; j >= 0; j--) {
		for (int i = 0; i < 5; i++) {
			if (!states[i][j]) {
				return aliens[i][j].getPosition().x;
			}
		}
	}
	return 0; // ovo mi ne bi trebalo trebati uopce - napravi provjeru u fji
}

void Aliens::nextLevel() {
	startSpeed += 0.2;
	speed = startSpeed;
	startY += 50;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (startSpeed == 3) {
				if (i == 0) {
					aliens[i][j].setTexture(alien3Txt);
				}
				else if (i < 3) {
					aliens[i][j].setTexture(alien2Txt);
				}
				else {
					aliens[i][j].setTexture(alien1Txt);
				}
			}
			else {
				states[i][j] = 0;
			}

			aliens[i][j].setScale(0.15, 0.15);
			aliens[i][j].setOrigin(aliens[i][j].getLocalBounds().width / 2, aliens[i][j].getLocalBounds().height / 2);
			aliens[i][j].setPosition(Vector2f(75 + j * 65, startY + i * 60));
		}
	}
}

int Aliens::allDestroyed() {
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 11; j++) {
			if (!states[i][j]) return 0;
		}
	}
	return 1;
}