#include "Bullet.h"

Bullet::Bullet(Object obj) {
	speed = 20;
	object = obj;

	bullet.setSize(Vector2f(1, 20));
	bullet.setFillColor((obj == Object::alien) ? Color(ALIEN_COLOR) : Color(PLAYER_COLOR));
	hide();
}

float Bullet::getSpeed() { // ovo ne koristim trenutno
	return speed;
}

void Bullet::render(MyWindow* w) {
	w->draw(bullet);
}

void Bullet::hide() {
	bullet.setScale(0, 0);
}

void Bullet::show(Vector2f position) {
	bullet.setScale(1, 1);
	bullet.setPosition(position);
}

int Bullet::isShowing() {
	if (bullet.getPosition().y >= 800 || bullet.getPosition().y <= 0) hide(); // sakrij ako je izvan ekrana
	return (bullet.getScale() == Vector2f(0, 0)) ? 0 : 1;
}

void Bullet::startShooting(Aliens* a, Player* p) {
	if (isShowing()) return;

	if (a) {
		show(a->getAlienPosition());
	}
	else {
		show(p->getLaserPosition() - Vector2f(0, 20));
	}
}

void Bullet::shoot(Defense* d, Aliens* a, Player* p) {
	if (!isShowing() && object == Object::player) return;
	if (!isShowing() && object == Object::alien) startShooting(a, p);

	Vector2f pos = bullet.getPosition();

	int tmp = 0;
	tmp = (object == Object::alien) ? p->isPlayerHit(pos) : a->isAlienHit(pos); // ovisno tko zove fju provjerava je li pogoden protivnik

	if (d->isHit(pos, object) // ako je pogodena obrana, prestani pucati
		|| tmp) {// ili ako je pogoden igrac (ako alien puca) ili ako je pogoden alien (ako igrac puca)
		hide();
		p->updateHit(tmp);
	}

	bullet.move(Vector2f(0, (object == Object::alien) ? speed : -speed));
}