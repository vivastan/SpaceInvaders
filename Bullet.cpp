#include "Bullet.h"
#include "Aliens.h"
#include "Player.h"

Bullet::Bullet(Color color) {
	speed = 0.35;
	direction = (color == Color::White) ? DOWN : UP;

	bullet.setSize(Vector2f(1, 20));
	bullet.setFillColor(color);
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
		show(p->getLaserPosition());
	}
}

void Bullet::shoot(Defense* d, Aliens* a, Player* p) {
	bullet.move(Vector2f(0, (direction == DOWN) ? speed : -speed));

	Vector2f pos = bullet.getPosition();

	if (d->isHit(pos) // ako je pogodena obrana, prestani pucati
		|| (direction == DOWN && p->isPlayerHit(pos)) // ili ako je pogoden igrac (ako alien puca)
		|| (direction == UP && a->isAlienHit(pos))) // ili ako je pogoden alien (ako igrac puca)
		hide();
}