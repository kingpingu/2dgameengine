#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
	private:
		bool isRunning;
		SDL_Window *window;
		SDL_Renderer *renderer;
	
	public:
		Game();
		~Game();
		bool getIsRunning() const;
		void initialize(int width, int height);
		void processInput();
		void update();
		void render();
		void destroy();
};

#endif
