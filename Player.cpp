#include "Player.h"

#define PLAYER_COLOR 198, 199, 191

Player::Player() : destroyer(Color(PLAYER_COLOR)) /*, bullet(Color::Green) */ {
	livesLeft.resize(3);
	restart();
	speed = 17;
	// wait = Time();
	direction = Direction::none;

	laserTexture.loadFromFile("laser.png");
	laser.setTexture(laserTexture);
	laser.setOrigin(laser.getLocalBounds().width / 2, laser.getLocalBounds().height / 2);
	laser.setPosition(Vector2f(400, 700));
	laser.setScale(0.1, 0.1);

	font.loadFromFile("arcade.ttf");

	setText(&textPoints, 50, 30);

	for (int i = 0; i < 3; i++) {
		livesLeft[i].setTexture(laserTexture);
		livesLeft[i].setScale(0.1, 0.1);
		livesLeft[i].setOrigin(livesLeft[i].getLocalBounds().width / 2, livesLeft[i].getLocalBounds().height / 2);
		livesLeft[i].setPosition(Vector2f(730 - i * 70, 40));
	}
}

/* ovdje ce ici kako se pomice
void Player::update(Aliens* a, Defense *d) {
	if (isShooting()) {
		bullet.shoot(d, a, NULL);
	}
} */

void Player::render(MyWindow* w) {
	w->draw(textPoints);
	// bullet.render(w);
	for (int i = 0; i < 3; i++)
		w->draw(livesLeft[i]);

	/*
	if (show && wait.asMilliseconds() == 0) {
		wait = time;
	}

	if ((time - wait).asMilliseconds() >= 200) {
		show = 0;
		wait = Time();
	}
	*/

	if (show) {
		destroyer.render(w);
	}
	else {
		w->draw(laser);
	}
}

int Player::showDestroy() {
	return show;
}

void Player::hideDestroy() {
	show = 0;
}

int Player::getLives() {
	return lives;
}

void Player::move() {
	if (direction == Direction::none) return;
	
	if (direction == Direction::right && laser.getPosition().x < 760) laser.move(Vector2f(speed, 0));
	else if (direction == Direction::left && laser.getPosition().x > 40) laser.move(Vector2f(-speed, 0));
}

/* dodano u Bullet
void Player::startShooting() {
	if (isShooting()) return; // ako vec puca nista
	// inace
	bullet.show(laser.getPosition());
	cerr << "pocinje pucati s " << laser.getPosition().x << ", " << laser.getPosition().y << endl;
} */

Vector2f Player::getLaserPosition() {
	return laser.getPosition();
}

void Player::restart() {
	lives = 3;
	points = 0;
	show = 0;

	for (int i = 0; i < 3; i++) {
		livesLeft[i].setScale(0.1, 0.1);
	}
	textPoints.setString("POINTS: 0");
}

/* sve preko Bullet
int Player::isShooting() {
	return bullet.isShowing();
} */

void Player::updateHit(int pts) {
	if (!pts) return;

	if (pts == -1) {
		lives--;
		cerr << lives << endl;
		destroyer.destroyed(laser.getPosition());
		show = 1;
		livesLeft[lives].setScale(0, 0);
		return;
	}

	// stopShooting();
	points += pts * 10;
	updatePoints();
}

int Player::isPlayerHit(Vector2f position) {
	float x = position.x, y = position.y;

	float down = laser.getPosition().y - laser.getLocalBounds().height * 0.1 / 2,
		up = laser.getPosition().y + laser.getLocalBounds().height * 0.1 / 2,
		left = laser.getPosition().x - laser.getLocalBounds().width * 0.1 / 2,
		right = laser.getPosition().x + laser.getLocalBounds().width * 0.1 / 2;

	if (down <= y && y <= up && left <= x && x <= right) {
		// cerr << x << ", " << y << "\t" << laser.getPosition().x - laser.getLocalBounds().width / 2 << " " << laser.getPosition().x + laser.getLocalBounds().width / 2 << endl;
		// updateHit(-1); -- ovo se radi vec u fji u Bullet pa da ne bude duplo
		return -1;
	}
	return 0;
}

void Player::updatePoints() {
	textPoints.setString("POINTS: " + to_string(points));
}

void Player::setText(Text* text, int x, int y) {
	// ovdje moze ici i font ako necu imati vise teksta + maknuti ga onda iz varijable u klasi
	text->setFont(font);
	text->setCharacterSize(20);
	text->setPosition(Vector2f(x, y));
}

void Player::setDirection(Direction dir) {
	direction = dir;
}

/* preko Bullet
void Player::stopShooting() {
	bullet.hide();
} */

float Player::getSpeed() {
	return speed;
}