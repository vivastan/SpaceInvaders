#include "Player.h"

Player::Player() : destroyer(Color(PLAYER_COLOR)) {
	livesLeft.resize(3);
	restart();
	speed = 900;

	laserTexture.loadFromFile("laser.png");

	initializeComponent(laser, Vector2f(400, 700));
	for (int i = 0; i < 3; i++) {
		initializeComponent(livesLeft[i], Vector2f(730 - i * 70, 40));
	}
}

void Player::render(MyWindow* w) {
	for (int i = 0; i < 3; i++)
		w->draw(livesLeft[i]);

	if (show) { /* prikazi animaciju za unistenje ako treba */
		destroyer.render(w);
	}
	else { /* inace prikazi top */
		w->draw(laser);
	}
}

/* pokazuje li se animacija unistenja */
int Player::showDestroy() {
	return show;
}

/* sakrij animaciju unistenja */
void Player::hideDestroy() {
	show = 0;
}

int Player::getLives() {
	return lives;
}

void Player::move(float distance) {
	if (direction == Direction::none) return;

	if (direction == Direction::right && laser.getPosition().x < 760) laser.move(Vector2f(distance, 0));
	else if (direction == Direction::left && laser.getPosition().x > 40) laser.move(Vector2f(-distance, 0));
}

Vector2f Player::getLaserPosition() {
	return laser.getPosition();
}

void Player::restart() {
	lives = 3;
	points = 0;
	show = 0;
	direction = Direction::none;

	for (int i = 0; i < 3; i++) {
		livesLeft[i].setScale(0.1, 0.1);
	}
}

/* updateaj po pogotku */
void Player::updateHit(int pts) {
	if (!pts) return;

	if (pts == -1) { /* pogoden je top */
		lives--;
		destroyer.destroyed(laser.getPosition());
		show = 1;
		livesLeft[lives].setScale(0, 0);
		return;
	}

	/* inace (pts > 0) pogoden je vanzemaljac - pribroji bodove */
	points += pts * 10;
}

/* provjerava je li pogoden top */
int Player::isPlayerHit(Vector2f position) {
	float x = position.x, y = position.y,
		width = laser.getLocalBounds().width * 0.1 / 2, height = laser.getLocalBounds().height * 0.1 / 2,
		posx = laser.getPosition().x, posy = laser.getPosition().y;

	if (posx - width <= x && x <= posx + width && posy - height <= y && y <= posy + height) {
		return -1;
	}
	return 0;
}


int Player::getPoints() {
	return points;
}

void Player::setDirection(Direction dir) {
	direction = dir;
}

float Player::getSpeed() {
	return speed;
}

void Player::initializeComponent(Sprite& s, Vector2f pos) {
	s.setTexture(laserTexture);
	s.setOrigin(s.getLocalBounds().width / 2, s.getLocalBounds().height / 2);
	s.setPosition(Vector2f(pos));
	s.setScale(0.1, 0.1);
}