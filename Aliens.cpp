#include "Aliens.h"

Aliens::Aliens() : destroyer(Color(ALIEN_COLOR)) {
	direction = Direction::left;

	alien1Txt.loadFromFile("alien1.png");
	alien2Txt.loadFromFile("alien2.png");
	alien3Txt.loadFromFile("alien3.png");

	aliens.resize(5, vector<Sprite>(11));

	restart(3, 125);
}

Aliens::~Aliens() {}

float Aliens::getSpeed() {
	return speed;
}

void Aliens::render(MyWindow* w) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			w->draw(aliens[i][j]);
		}
	}

	if (destroyer.isShowing()) { /* ako se prikazuje animacija unistenja, nacrtaj i nju */
		destroyer.render(w);
	}
}

void Aliens::move(Defense* d) {
	Vector2f offset((direction == Direction::left) ? -speed : speed, 0); /* pomak ovisno o smjeru */

	if (getLeftPosition() <= 25) { /* ako je najlijeviji vanzemaljac dosao do najlijevije tocke */
		direction = Direction::right; /* promijeni smjer */
		offset.y += 2; /* pomakni prema dolje */
		speed += 0.1f; /* povecaj brzinu */
	}
	else if (getRightPosition() >= 775) { /* ako je najdesniji vanzemaljac dosao do najdesnije tocke */
		direction = Direction::left; /* promijeni smjer */
		offset.y += 2; /* pomakni prema dolje */
		speed += 0.1f; /* povecaj brzinu */
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			aliens[i][j].move(offset); /* pomakni */

			if (!isHit(i, j)) /* ako nije pogoden, provjera je li dotaknuo neki obrambeni objekt */
				d->onCollision(aliens[i][j].getPosition());
		}
	}
}

/* provjerava je li vanzemaljac pogoden */
int Aliens::isAlienHit(Vector2f position) {
	float x = position.x, y = position.y,
		width = aliens[0][0].getLocalBounds().width * 0.15 / 2, height = aliens[0][0].getLocalBounds().height * 0.15 / 2;
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 11; j++) {
			if (isHit(i, j)) continue; /* ako je vec ranije pogoden, preskace se */

			float posx = aliens[i][j].getPosition().x, posy = aliens[i][j].getPosition().y;
			if (posx - width <= x && x <= posx + width &&
				((posy - height <= y && y <= posy + height) || (posy - height <= y + 20 && y + 20 <= posy + height))) {
				return onHit(i, j);
			}
		}
	}
	return 0;
}

/* vraca poziciju najnizeg nepogodenog vanzemaljca - za provjeru je li dotakao dno ekrana */
float Aliens::getLowestAlienPosition() {
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 11; j++) {
			if (!isHit(i, j)) {
				return aliens[i][j].getPosition().y;;
			}
		}
	}
	return -1; /* do ovog ne bi trebalo uopce doci */
}

Vector2f Aliens::getAlienPosition() {
	vector<pair<int, int>> v;

	/* trazi najnize vanzemaljce za svaki stupac */
	for (int j = 0; j < 11; j++) {
		int i = 4;
		for (; i >= 0; i--) {
			if (!isHit(i, j)) {
				v.push_back(pair<int, int>(i, j));
				break;
			}
		}
	}

	if (v.size() == 0) return Vector2f(-1.f, -1.f);

	int t = rand() % v.size();

	/* vrati poziciju nekog najnizeg vanzemaljca (na random) */
	return aliens[v[t].first][v[t].second].getPosition();
}

/* vraca 1 ako su svi vanzemaljci unisteni, 0 inace */
int Aliens::allDestroyed() {
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 11; j++) {
			if (!isHit(i, j)) return 0;
		}
	}
	/* ako su svi unisteni, odmah pozovi restart da se generiraju novi, ali s vecom pocetnom brzinom i nizom pocetnom pozicijom */
	restart(startSpeed + 0.2, startY + 50);
	return 1;
}

/* pokazuje li se animacija unistenja */
int Aliens::isDestroyShowing() {
	return destroyer.isShowing();
}

/* sakrij animaciju unistenja */
void Aliens::hideDestroy() {
	destroyer.hide();
}

/* (re)start igre */
void Aliens::restart(int _startSpeed, int _startY) {
	startSpeed = _startSpeed;
	startY = _startY;
	speed = startSpeed;

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

			aliens[i][j].setScale(0.15, 0.15);
			aliens[i][j].setOrigin(aliens[i][j].getLocalBounds().width / 2, aliens[i][j].getLocalBounds().height / 2);
			aliens[i][j].setPosition(Vector2f(75 + j * 65, startY + i * 60));
		}
	}
	destroyer.hide();
}

int Aliens::isHit(int x, int y) {
	return (aliens[x][y].getScale() == Vector2f(0, 0)) ? 1 : 0;
}

/* kad je vanzemaljac pogoden, prikazuje se animacija unistavanja + vanzemaljac se vise ne prikazuje */
/* vraca 1 / 2 / 3 ovisno u kojem redu je (koliko vrijedi) pogodeni vanzemaljac */
int Aliens::onHit(int x, int y) {
	destroyer.destroyed(aliens[x][y].getPosition());
	aliens[x][y].setScale(0, 0);

	if (x == 0) return 3;
	else if (x < 3) return 2;
	return 1;
}

/* vraca najlijeviju poziciju nepogodenih vanzemaljaca */
int Aliens::getLeftPosition() {
	for (int j = 0; j < 11; j++) {
		for (int i = 0; i < 5; i++) {
			if (!isHit(i, j)) {
				return aliens[i][j].getPosition().x;
			}
		}
	}
	return -1; /* do ovog ne bi trebalo doci uopce */
}

/* vraca najdesniju poziciju nepogodenih vanzemaljaca */
int Aliens::getRightPosition() {
	for (int j = 10; j >= 0; j--) {
		for (int i = 0; i < 5; i++) {
			if (!isHit(i, j)) {
				return aliens[i][j].getPosition().x;
			}
		}
	}
	return 801; /* do ovog ne bi trebalo doci uopce */
}