#include "Bullet.h"

Bullet::Bullet(Object obj) {
	speed = 20;
	object = obj;

	bullet.setSize(Vector2f(1, 20));
	bullet.setFillColor((obj == Object::alien) ? Color(ALIEN_COLOR) : Color(PLAYER_COLOR));
	bullet.setOrigin(0.5, 0);
	hide();
}

float Bullet::getSpeed() {
	return speed;
}

void Bullet::render(MyWindow* w) {
	w->draw(bullet);
}

void Bullet::hide() {
	bullet.setScale(0, 0);
}

/* prikazi (na pocetnoj poziciji) */
void Bullet::show(Vector2f position) {
	bullet.setScale(1, 1);
	bullet.setPosition(position);
}

/* vraca 1 ako se prikazuje, inace 0 */
int Bullet::isShowing() {
	if (bullet.getPosition().y >= 800 || bullet.getPosition().y <= 0)
		hide(); /* sakrij ako je izvan ekrana */
	return (bullet.getScale() == Vector2f(0, 0)) ? 0 : 1;
}

void Bullet::startShooting(Aliens* a, Player* p) {
	if (isShowing()) return; /* ako vec puca, nista */

	if (a) { /* ako vanzemaljac puca, pucaj s njegove trenutne pozicije */
		show(a->getAlienPosition());
	}
	else { /* ako top puca, pucaj s njegove trenutne pozicije */
		show(p->getLaserPosition() - Vector2f(0, 20));
	}
}

void Bullet::shoot(Defense* d, Aliens* a, Player* p) {
	if (!isShowing() && object == Object::player)
		return; /* top puca samo kad igrac pritisne space (tad je metak vec prikazan) */
	if (!isShowing() && object == Object::alien)
		startShooting(a, p); /* vanzemaljci pucaju cim je 'prethodni' metak unisten */

	Vector2f pos = bullet.getPosition();

	int tmp = 0;
	/* ovisno tko zove fju provjerava je li pogoden protivnik
		obje fje vracaju 0 ako nije pogoden, inace != 0 (i medusobno razlicite vrijednosti) */
	tmp = (object == Object::alien) ? p->isPlayerHit(pos) : a->isAlienHit(pos);

	if (d->isHit(pos, object) /* ako je pogodena obrana, prestani pucati */
		|| tmp) { /* ili ako je pogoden igrac ili vanzemaljac, ovisno tko puca */
		hide();
		p->updateHit(tmp); /* ako je tmp = 0, nista nece napraviti, inace ovisno tko je pogoden updatea igru */
	}

	bullet.move(Vector2f(0, (object == Object::alien) ? speed : -speed));
}