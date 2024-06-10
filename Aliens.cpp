#include "Aliens.h"
#include "Player.h"
#include "Bullet.h"

Aliens::Aliens() : destroyer(), bullet(Color::White) {
	direction = LEFT;
	show = 0;

	speed = 3;

	alien1Txt.loadFromFile("alien1.png");
	alien2Txt.loadFromFile("alien2.png");
	alien3Txt.loadFromFile("alien3.png");

	states.resize(5, vector<int>(11, 0));
	aliens.resize(5, vector<Sprite>(11));
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
}

Aliens::~Aliens() {}

float Aliens::getSpeed() {
	return speed;
}

void Aliens::render(MyWindow* w, Time time) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			w->draw(aliens[i][j]);
		}
	}
	bullet.render(w);

	if (show) {
		destroyer.render(w);
	}
}

float Aliens::getBulletSpeed() { // trenutno nicem ne sluzi
	return bullet.getSpeed();
}

void Aliens::update(Player* p, Defense* d) {
	if (!isShooting()) {
		startShooting();
	}
	else {
		bullet.shoot(d, NULL, p);
	}
}

void Aliens::move(Defense* d) {
	Vector2f offset((direction == LEFT) ? -speed : speed, 0);

	if (getLeftPosition() <= 25) {
		direction = RIGHT;
		offset.y += 2;
		speed += 0.1;
	}
	else if (getRightPosition() >= 775) {
		direction = LEFT;
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
					while (d->onHit(k, DOWN));
					break;
				}
			}
		}
	}

	show = 0;
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

void Aliens::startShooting() {
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

	if (v.size() == 0) return;

	int t = rand() % v.size();

	// cerr << "alien koji puca: " << v[t].first << ", " << v[t].second << endl;

	bullet.show(aliens[v[t].first][v[t].second].getPosition());
}

int Aliens::isAlienHit(Vector2f position) {
	int x = position.x, y = position.y;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (states[i][j]) continue;

			if ((int)(aliens[i][j].getPosition().y - aliens[i][j].getLocalBounds().height * 0.15 / 2) == y
				&& (int)(aliens[i][j].getPosition().x - aliens[i][j].getLocalBounds().width * 0.15 / 2) <= x
				&& x <= (int)(aliens[i][j].getPosition().x + aliens[i][j].getLocalBounds().width * 0.15 / 2)) {
				cerr << i << ", " << j << endl;
				return onHit(i, j);
			}
		}
	}
	return 0;
}

int Aliens::isShooting() {
	return bullet.isShowing();
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