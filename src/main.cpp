#include "./constants.h"
#include "./game.h"

int main(int argc, char *argv[]) {
	Game *game = new Game();

	game->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (game->getIsRunning()) {
		game->processInput();
		game->update();
		game->render();
	}

	game->destroy();

	return 0;
}
