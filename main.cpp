#include "Game.h"

int main() {
	Game game;
	while (game.getWindow()->isOpen()) {
		game.processInput();
		game.update();
		game.render();
		game.restartClock();
	}
	return 0;
}