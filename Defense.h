#ifndef DEFENSE_H
#define DEFENSE_H

#include "SFML/Graphics.hpp"
#include "Enums.h"

#include <iostream>

using namespace sf;
using namespace std;

class Defense {
public:
	Defense() {
		defenseTexture.loadFromFile("defense.png");

		defense.resize(4);
		for (int i = 0; i < 4; i++) {
			defense[i].setTexture(defenseTexture);
			defense[i].setOrigin(defense[i].getLocalBounds().width / 2, defense[i].getLocalBounds().height / 2);
			defense[i].setPosition(Vector2f(100 + i*200, 600));
			defense[i].setScale(Vector2f(0.2f, 0.2f));
		}

		destroyed.resize(4, vector<int>(12, 0)); // 0 ako pravokutnik nije postavljen, 1 ako je

		float width = defense[0].getLocalBounds().width * 0.2, height = defense[0].getLocalBounds().height * 0.2;
		defenseDestroyed.resize(4, vector<RectangleShape>(12)); // za 4 obrambena objekta svaki se unistava s 12 metaka
		for (int i = 0; i < 4; i++) {
			// std::cerr << "i: " << i << endl;
			for (int j = 0; j < 12; j++) {
				defenseDestroyed[i][j].setSize(Vector2f(width/3, height/4));
				defenseDestroyed[i][j].setFillColor(Color::Black);
				defenseDestroyed[i][j].setPosition(Vector2f(100 - width / 2 + (j%3) * width/3 + i*200, 600 - height/2 + j/3 * height/4));
				defenseDestroyed[i][j].setScale(0, 0);

				/*
				if (i == 0) {
					if (j == 0) std::cerr << "w = " << width << ", h = " << height << std::endl;
					float dX = defenseDestroyed[i][j].getPosition().x, dY = defenseDestroyed[i][j].getPosition().y;
					std::cerr << dX << " " << dX + width / 3 << ", " << dY << " " << dY + height / 4 << std::endl;
				}
				*/
			}
		}
	}

	void render(MyWindow* w) {
		for (int i = 0; i < 4; i++) {
			w->draw(defense[i]);
			for (int j = 0; j < 12; j++) {
				w->draw(defenseDestroyed[i][j]);
			}
		}
	}

	int isHit(Vector2f position, Object obj) { // staviti da se provjerava jel ovaj gore / dolje unisten
		float x = position.x, y = position.y, width = defense[0].getLocalBounds().width * 0.2, height = defense[0].getLocalBounds().height * 0.2;
		for (int i = 0; i < 4; i++) {
			// float posx = defense[i].getPosition().x;
			for (int j = 0; j < 12; j++) {
				if (destroyed[i][j]) continue; // vec je unisten

				float dX = defenseDestroyed[i][j].getPosition().x, dY = defenseDestroyed[i][j].getPosition().y;
				
				if (dX <= x && x <= dX + width / 3 && dY <= y && y <= dY + height / 4) {
					if (obj == Object::alien && j >= 3 && !destroyed[i][j - 3]) {
						onDestroy(i, j - 3);
					}
					else if (obj == Object::player && j <= 8 && !destroyed[i][j + 3]) {
						onDestroy(i, j + 3);
					}
					else {
						onDestroy(i, j);
					}
					// std::cerr << "pravokuntik " << i << ", " << j << endl;
					// std::cerr << x << ", " << y << std::endl;
					return 1;
				}
			}
		}
		return 0;
	}

	/* ne koristim -- sto znaci da mi ne treba enum onaj gore */
	int onHit(int i, Object obj) { // 1 ako je pogodena, 0 ako je cijele nema vec
		if (obj == Object::player) {
			for (int j = 11; j >= 0; j--) {// za playera ide odozdola
				if (defenseDestroyed[i][j].getScale() == Vector2f(0, 0)) {
					defenseDestroyed[i][j].setScale(1, 1);
					return 1;
				}
			}
		}
		else {
			for (int j = 0; j < 12; j++) {// za playera ide odozdola
				if (defenseDestroyed[i][j].getScale() == Vector2f(0, 0)) {
					defenseDestroyed[i][j].setScale(1, 1);
					return 1;
				}
			}
		}
		return 0;
	}

	Sprite& getDefense(int i) {
		return defense[i];
	}

	void nextLevel() { // ne koristi se jos ali to je za restart
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 12; j++) {
				destroyed[i][j] = 0;
				defenseDestroyed[i][j].setScale(0, 0);
			}
		}
	}

private:
	Texture defenseTexture;
	vector<Sprite> defense;
	vector<vector<int>> destroyed;
	vector<vector<RectangleShape>> defenseDestroyed;

	void onDestroy(int i, int j) {
		destroyed[i][j] = 1;
		defenseDestroyed[i][j].setScale(1, 1);
	}
};
#endif // !DEFENSE_H