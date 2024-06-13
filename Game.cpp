#include "Game.h"

Game::Game() : w(), p(), a(), d(), aBullet(Object::alien), pBullet(Object::player) {
	play = GameFlow::menu; /* pocetno se prikazuje izbornik */

	time1 = Time::Zero;
	time2 = Time::Zero;
	time3 = Time::Zero;
	time4 = Time::Zero;
	time5 = Time::Zero;

	font.loadFromFile("arcade.ttf");

	setText(menuText, 400, 400);
	menuText.setString("PRITISNI BILO GDJE DA IGRA POCNE");
	menuText.setOrigin(menuText.getLocalBounds().width / 2, menuText.getLocalBounds().height / 2);

	setText(pointsText, 50, 30);
}
Game::~Game() {}

MyWindow* Game::getWindow() {
	return &w;
}
void Game::processInput() {
	if (play == GameFlow::playerHit) {
		return;
	}
	else if (play == GameFlow::menu || play == GameFlow::over) {
		processMenuAction();
		return;
	}

	handleKeyboardInput();
}

void Game::update() {
	w.update();

	if (isGameOver()) { /* ako je gotova igra */
		play = GameFlow::over;
		menuText.setString("KRAJ IGRE\nPRITISNI BILO GDJE DA\nNOVA IGRA POCNE");
		return;
	}

	if (play == GameFlow::restart && time1.asSeconds() >= 1) {
		play = GameFlow::on;
		time1 = Time::Zero;
	}
	else if (play == GameFlow::on) {
		if (a.allDestroyed()) { /* ako su svi vanzemaljci unisteni, restart igre s vecom brzinom i nizom pozicijom */
			restart();
			return;
		}

		moveObjects();

		/* update bodova */
		pointsText.setString("POINTS: " + to_string(p.getPoints()));
	}
	else if (play == GameFlow::playerHit) {
		if (time5.asSeconds() >= 1) {
			time5 = Time::Zero;
			play = GameFlow::on;
			p.hideDestroy();
		}
	}
}

void Game::render() {
	w.clear();

	if (play == GameFlow::on || play == GameFlow::playerHit)
		drawPlay();
	else if (play == GameFlow::menu || play == GameFlow::over)
		drawMenu();

	w.show();
}

void Game::restartClock() {
	time = clock.restart();

	if (play == GameFlow::on) {
		time1 += time;
		time2 += time;
		time3 += time;
		time4 += time;
	}
	else if (play == GameFlow::restart) {
		time1 += time;
	}
	else if (play == GameFlow::playerHit) {
		time1 = Time::Zero;
		time2 = Time::Zero;
		time3 = Time::Zero;
		time4 = Time::Zero;
		time5 += time;
	}
}

Time Game::elapsedTime() { // ne koristi se ionako ?
	return time;
}

/* osnovno postavljanje teksta */
void Game::setText(Text& text, int x, int y) {
	text.setFont(font);
	text.setCharacterSize(20);
	text.setPosition(Vector2f(x, y));
}

int Game::isGameOver() {
	/* igra je gotova ako igrac izgubi sve zivote */
	if (!p.getLives()) return 1;

	/* ili ako vanzemaljci dodu do dna */
	if (a.getLowestAlienPosition() >= 780) return 1;

	return 0;
}

/* crtanje kad je igra u tijeku */
void Game::drawPlay() {
	w.draw(pointsText);
	d.render(&w);
	p.render(&w);
	a.render(&w);
	pBullet.render(&w);
	aBullet.render(&w);
}

void Game::drawMenu() {
	// w.draw(p.getText()); // ne treba mi ovo valjda to su bodovi
	w.draw(menuText);
}

void Game::processMenuAction() {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		float x = Mouse::getPosition(w.getWindow()).x, y = Mouse::getPosition(w.getWindow()).y;
		if (x >= 0 && x <= 800 && y >= 0 && y <= 800) {
			time = clock.restart();
			if (play == GameFlow::over) {
				p.restart();
				a.restart(3, 125);
				aBullet.hide();
				pBullet.hide();
				d.restart();
			}
			pointsText.setString("POINTS: 0");
			play = GameFlow::on;
		}
	}
}

/* input od igraca */
void Game::handleKeyboardInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		p.setDirection(Direction::left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		p.setDirection(Direction::right);
	}
	else {
		p.setDirection(Direction::none);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		pBullet.startShooting(NULL, &p);
	}
}

void Game::restart() {
	aBullet.hide();
	pBullet.hide();
	d.restart();
	/* ne treba restart vanzemaljaca jer se to radi u allDestroyed() fji ako vraca 1 */

	/* vrijeme se vraca na nulu */
	time1 = Time::Zero;
	time2 = Time::Zero;
	time3 = Time::Zero;
	time4 = Time::Zero;

	play = GameFlow::restart;
}

void Game::moveObjects() {
	/* pomicanje vanzemaljaca */
	float iterTime1 = 1.0f / a.getSpeed();
	if (time1.asSeconds() >= iterTime1) {
		a.move(&d);
		time1 -= sf::seconds(iterTime1);
	}

	/* pomicanje metaka */
	float iterTime2 = 1.0f / aBullet.getSpeed();
	if (time2.asSeconds() >= iterTime2) {
		pBullet.shoot(&d, &a, &p);
		aBullet.shoot(&d, &a, &p);
		time2 -= sf::seconds(iterTime2);
	}

	/* pomicanje topa */
	float iterTime3 = 1000.0f / p.getSpeed();
	if (time3.asMilliseconds() >= iterTime3) {
		p.move(p.getSpeed() / 1000.f * time.asMilliseconds());
		time3 -= sf::milliseconds(iterTime3);
	}

	/* animacija unistenja vanzemaljaca */
	if (!a.isDestroyShowing()) {
		time4 = Time::Zero;
	}
	else if (a.isDestroyShowing() && time4.asSeconds() >= iterTime1) {
		/* brzina kojom se animacija makne ovisi o brzini kojom se vanzemaljci krecu */
		a.hideDestroy();
		time4 -= sf::seconds(iterTime1);
	}

	/* ako je top pogoden, tj prikazuje se njegova animacija unistenja */
	if (p.showDestroy()) {
		play = GameFlow::playerHit;
		pBullet.hide();
		aBullet.hide();
	}
}