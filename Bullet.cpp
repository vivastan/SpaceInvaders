#include "Bullet.h"

Bullet::Bullet(Object obj) {
	speed = 20;
	object = obj;

	bullet.setSize(Vector2f(1, 20));
	bullet.setFillColor((obj == Object::alien) ? Color(ALIEN_COLOR) : Color(PLAYER_COLOR));
	// bullet.setOrigin(0.5, (direction == DOWN) ? 20 : 0);
	/*
	if (direction == UP) {
		bullet.setPosition(20, 500);
		cerr << "test bullet position " << bullet.getPosition().x << ", " << bullet.getPosition().y - bullet.getLocalBounds().height/2 << endl;
	} */
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

Vector2f Bullet::getPosition() {
	return bullet.getPosition();
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

	bullet.move(Vector2f(0, (object == Object::alien) ? speed : -speed));

	Vector2f pos = bullet.getPosition();

	/*
	if (direction == UP)
		cerr << "bullet position = " << pos.x << ", " << pos.y << endl;
	*/

	int tmp = 0;
	tmp = (object == Object::alien) ? p->isPlayerHit(pos) : a->isAlienHit(pos);

	if (d->isHit(pos, object) // ako je pogodena obrana, prestani pucati
		|| tmp) {// ili ako je pogoden igrac (ako alien puca) ili ako je pogoden alien (ako igrac puca)
		hide();
		p->updateHit(tmp);
	}

	// return tmp;
}