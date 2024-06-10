#include "Player.h"
#include "Aliens.h" // Include Aliens.h to use Aliens class
#include "Bullet.h"

Player::Player() : destroyer(), bullet(Color::Green) {
	lives = 3;
	points = 0;
	show = 0;
	speed = 10;
	wait = Time();

	laserTexture.loadFromFile("laser.png");
	laser.setTexture(laserTexture);
	laser.setOrigin(laser.getLocalBounds().width / 2, laser.getLocalBounds().height / 2);
	laser.setPosition(Vector2f(400, 700));
	laser.setScale(0.1, 0.1);

	font.loadFromFile("arcade.ttf");

	setText(&textPoints, 50, 30);
	textPoints.setString("POINTS: 0");

	livesLeft.resize(3);
	for (int i = 0; i < 3; i++) {
		livesLeft[i].setTexture(laserTexture);
		livesLeft[i].setScale(0.1, 0.1);
		livesLeft[i].setOrigin(livesLeft[i].getLocalBounds().width / 2, livesLeft[i].getLocalBounds().height / 2);
		livesLeft[i].setPosition(Vector2f(730 - i * 70, 40));
	}
}

void Player::update(Aliens* a, Defense *d) {
	if (isShooting()) {
		bullet.shoot(d, a, NULL);
	}
}

void Player::render(MyWindow* w, Time time) {
	w->draw(textPoints);
	bullet.render(w);
	for (int i = 0; i < 3; i++)
		w->draw(livesLeft[i]);

	if (show && wait.asMilliseconds() == 0) {
		wait = time;
	}

	if ((time - wait).asMilliseconds() >= 200) {
		show = 0;
		wait = Time();
	}

	if (show) {
		destroyer.render(w);
	}
	else {
		w->draw(laser);
	}
}

int Player::getLives() {
	return lives;
}

void Player::move(int direction) {
	if (direction > 0 && laser.getPosition().x < 780) laser.move(Vector2f(0.2, 0));
	else if (laser.getPosition().x > 20) laser.move(Vector2f(-0.2, 0));
}

void Player::startShooting() {
	if (isShooting()) return; // ako vec puca nista
	// inace
	bullet.show(laser.getPosition());
	cerr << "pocinje pucati s " << laser.getPosition().x << ", " << laser.getPosition().y << endl;
}

Vector2f Player::getLaserPosition() {
	return laser.getPosition();
}

int Player::isShooting() {
	return bullet.isShowing();
}

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

	stopShooting();
	points += pts * 10;
	updatePoints();
}

int Player::isPlayerHit(Vector2f position) {
	int x = position.x, y = position.y;

	if (y == laser.getPosition().y
		&& (laser.getPosition().x - laser.getLocalBounds().width * 0.1 / 2) <= x
		&& x <= (laser.getPosition().x + laser.getLocalBounds().width * 0.1 / 2)) {
		// cerr << x << ", " << y << "\t" << laser.getPosition().x - laser.getLocalBounds().width / 2 << " " << laser.getPosition().x + laser.getLocalBounds().width / 2 << endl;
		updateHit(-1);
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

void Player::stopShooting() {
	bullet.hide();
}

float Player::getSpeed() {
	return speed;
}