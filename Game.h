#include "MyWindow.h"
#include "Player.h"
#include "Aliens.h"
#include "Bullet.h"
#include "Defense.h"

#include <iostream>

// TODO: dodati vrijeme kao u slideovima
//	- to je to u principu al ne svida mi se da se top za tak puno pomakne + ja bi da to izgl glade
// TODO: ok je sad za playera kad je pogoden, al ja ne bi da se pauzira i nastavi dalje kako je stalo vec da svi metci se prestanu pucati
// TODO: popraviti za kad je top blizu ruba
// TODO: obrambeni objekti se uniste ako ih dotakne alien -- dodala sam to ali nisam istestirala jer prije uniste ga ovako
// TODO: za obrambene objekte nije mi dobar ovaj u sredini sto se unisti uopce se ne vidi
//			-- mogu to inicijalizirati i odmah postaviti da se pokrije s pravokutnikom
// TODO: mytery ship (mozda)
// TODO: high score ako ce mi falit bodova

class Game {
public:
	Game() : w(), p(), a(), d(), aBullet(Object::alien), pBullet(Object::player) {
		play = GameFlow::menu;

		time1 = Time::Zero;
		time2 = Time::Zero;
		time3 = Time::Zero;
		time4 = Time::Zero;
		time5 = Time::Zero;

		font.loadFromFile("arcade.ttf");
		text.setFont(font);
		text.setString("PRITISNI BILO GDJE DA IGRA POCNE");
		text.setCharacterSize(22);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(400, 400);
	}
	~Game() {}

	MyWindow* getWindow() {
		return &w;
	}
	void processInput() {

		if (play == GameFlow::over || play == GameFlow::playerHit) return;

		if (play == GameFlow::menu) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				float x = Mouse::getPosition(w.getWindow()).x, y = Mouse::getPosition(w.getWindow()).y;
				if (x >= 0 && x <= 800 && y >= 0 && y <= 800)
					time = clock.restart();
					play = GameFlow::on;
			}
			return;
		}

		if (isGameOver()) {
			play = GameFlow::over;
			text.setString("KRAJ IGRE\nPRITISNI BILO GDJE DA\nNOVA IGRA POCNE");
			return;
		}

		// std::cerr << time.asMilliseconds() << std::endl;
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
	void update() {
		w.update();
		
		if (play == GameFlow::restart && time1.asSeconds() >= 1) {
			play = GameFlow::on;
			time1 = Time::Zero;
		}
		else if (play == GameFlow::on) {
			if (a.allDestroyed()) {
				// restart igre
				a.nextLevel();
				aBullet.hide();
				pBullet.hide();
				d.nextLevel();

				// time = Time::Zero; // mislim da ne treba
				time1 = Time::Zero;
				time2 = Time::Zero;
				time3 = Time::Zero;
				time4 = Time::Zero;

				play = GameFlow::restart;

				return;
			}

			float iterTime1 = 1.0f / a.getSpeed();
			if (time1.asSeconds() >= iterTime1) {
				a.move(&d);
				time1 -= sf::seconds(iterTime1);
			}

			float iterTime2 = 1.0f / aBullet.getSpeed();
			if (time2.asSeconds() >= iterTime2) {
				pBullet.shoot(&d, &a, &p);
				aBullet.shoot(&d, &a, &p);
				time2 -= sf::seconds(iterTime2);
			}

			float iterTime3 = 1000.0f / p.getSpeed(); // mozda za playera da ide u milisekundama
			if (time3.asMilliseconds() >= iterTime3) {
				p.move();
				time3 -= sf::milliseconds(iterTime3);
			}

			if (!a.showDestroy()) {
				time4 = Time::Zero;
			}
			
			if (a.showDestroy() && time4.asMilliseconds() >= 200) {
				// pokazi animaciju za unistenje ako treba
				a.hideDestroy();
			}

			if (p.showDestroy()) {
				play = GameFlow::playerHit;
			}
		}
		else if (play == GameFlow::playerHit) {
			if (time5.asSeconds() >= 1) {
				time5 = Time::Zero;
				play = GameFlow::on;
				p.hideDestroy();
			}
		}
	}

	void render() {
		w.clear();

		if (play == GameFlow::on || play == GameFlow::playerHit) drawPlay();
		else if (play == GameFlow::menu || play == GameFlow::over) drawGameOver();

		w.show();
	}

	void restartClock() {
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

	Time elapsedTime() { // ne koristi se ionako
		return time;
	}

private:
	Clock clock;
	Time time;
	Time time1;
	Time time2;
	Time time3;
	Time time4;
	Time time5;
	MyWindow w;
	Player p;
	Aliens a;
	Defense d;
	GameFlow play;
	Font font;
	Text text;
	Bullet aBullet;
	Bullet pBullet;

	int isGameOver() {
		// ako igrac izgubi sve zivote
		if (!p.getLives()) return 1;

		// ili ako alieni dodu do dna
		if (a.getLowestAlienPosition() >= 780) return 1;

		return 0;
	}

	void drawPlay() {
		d.render(&w);
		p.render(&w);
		a.render(&w);
		pBullet.render(&w);
		aBullet.render(&w);
	}

	void drawGameOver() {
		// w.draw(p.getText()); // ne treba mi ovo valjda to su bodovi
		w.draw(text);
	}
};