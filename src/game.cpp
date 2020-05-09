#include <iostream>
#include "./constants.h"
#include "./game.h"
#include "./assetManager.h"
#include "./components/transformComponent.h"
#include "./components/spriteComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;

Game::Game() {
	isRunning = false;
}

Game::~Game() {

}

bool Game::getIsRunning() const {
	return isRunning;
}

void Game::initialize(int width, int height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
	}
	
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}
	
	loadLevel(0);

	isRunning = true;
	return;
}

void Game::loadLevel(int levelNumber) {
	assetManager->addTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
	assetManager->addTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
	
	Entity& tankEntity(manager.addEntity("tank"));
	tankEntity.addComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
	tankEntity.addComponent<SpriteComponent>("tank-image");

	Entity& chopperEntity(manager.addEntity("chopper"));
	chopperEntity.addComponent<TransformComponent>(240, 106, 0 , 0, 32, 32, 1);
	chopperEntity.addComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
}

void Game::processInput() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		default:
			break;
	}
}

void Game::update() {
	// wait until 16ms has elapsed since the last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));
	
	// delta time is the difference in ticks from last frame converted to seconds
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	// clamp deltaTime to a maximum value
	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

	// sets the new ticks for the current frame to be used in the next pass
	ticksLastFrame = SDL_GetTicks();
	
	manager.update(deltaTime);
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	if (manager.hasNoEntities()) {
		return;
	}

	manager.render();

	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
