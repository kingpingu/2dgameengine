#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/lua/sol.hpp"
#include "./entity.h"
#include "./component.h"
#include "./entityManager.h"

class AssetManager;

class Game {
	private:
		bool isRunning;
		SDL_Window *window;
	
	public:
		Game();
		~Game();
		int ticksLastFrame;
		bool getIsRunning() const;
		static SDL_Renderer *renderer;
		static AssetManager* assetManager;
		static SDL_Event event;
		static SDL_Rect camera;
		void loadLevel(int levelNumber);
		void initialize(int width, int height);
		void processInput();
		void update();
		void render();
		void destroy();
		void handleCameraMovement();
		void checkCollisions();
		void processNextLevel(int levelNumber);
		void processGameOver();
};

#endif
