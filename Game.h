#include "MyWindow.h"
#include "Player.h"
#include "Aliens.h"
#include "Defense.h"

#include <iostream>

#define UP 1
#define DOWN 2 // enum

// TODO: dodati vrijeme kao u slideovima - chatgpt
// TODO: obrambeni objekti se uniste ako ih dotakne alien -- dodala sam to ali nisam istestirala jer prije uniste ga ovako
// TODO: za obrambene objekte nije mi dobar ovaj u sredini sto se unisti uopce se ne vidi
//			-- mogu to inicijalizirati i odmah postaviti da se pokrije s pravokutnikom
// TODO: animacija za aliene kad se pomicu
// TODO: ne pokaze se svaki put animacija kad je alien pogoden jer se poziva ta fja samo svakih toliko pa ako se bas pogodi onda jbg
// TODO: da se restarta igra
// TODO: mytery ship (mozda)

class Game {
public:
	Game() : w(), p(), a(), d(), aBullet(Color::White), pBullet(Color::Green) {
		play = -1;
		bulletSpeed = 35;
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

		if (play == 0) return;

		if (play == -1) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				float x = Mouse::getPosition(w.getWindow()).x, y = Mouse::getPosition(w.getWindow()).y;
				if (x >= 0 && x <= 800 && y >= 0 && y <= 800)
					time = clock.restart();
					play = 1;
			}
			return;
		}

		if (isGameOver()) {
			play = 0;
			text.setString("KRAJ IGRE\nPRITISNI BILO GDJE DA\nNOVA IGRA POCNE");
			return;
		}

		// std::cerr << time.asMilliseconds() << std::endl;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			p.move(-1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			p.move(1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			// p.startShooting();
			pBullet.startShooting(NULL, &p);
		}
	}
	void update() {
		w.update();
		
		if (play == 1) {
			p.update(&a, &d);
			a.update(&p, &d);
			pBullet.shoot(&d, &a, &p);
			aBullet.shoot(&d, &a, &p);

			float iterTime = 1.0f / a.getSpeed();
			if (time.asSeconds() >= iterTime) {
				a.move(&d);
				time -= sf::seconds(iterTime);
			}

			if (time.asSeconds() >= 1.0f / p.getSpeed()) {

			}
		}
	}

	void render() {
		w.clear();

		if (play == 1) drawPlay();
		else drawGameOver();

		w.show();
	}

	void restartClock() {
		time += clock.restart();
	}

	Time elapsedTime() {
		return time;
	}

private:
	Clock clock;
	Time time;
	MyWindow w;
	Player p;
	Aliens a;
	Defense d;
	int play;
	Font font;
	Text text;
	float bulletSpeed;
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
		p.render(&w, time);
		a.render(&w, time);
	}

	void drawGameOver() {
		// w.draw(p.getText()); // ne treba mi ovo valjda to su bodovi
		w.draw(text);
	}
};