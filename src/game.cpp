#include <iostream>
#include "./constants.h"
#include "./game.h"
#include "./assetManager.h"
#include "./map.h"
#include "./components/transformComponent.h"
#include "./components/spriteComponent.h"
#include "./components/colliderComponent.h"
#include "./components/keyboardControlComponent.h"
#include "./components/textLabelComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;

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
		return;
	}

	if (TTF_Init() != 0) {
		std::cerr << "Error initializing SDL TTF." << std::endl;
		return;
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

Entity& player(manager.addEntity("chopper", PLAYER_LAYER));

void Game::loadLevel(int levelNumber) {
	assetManager->addTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
	assetManager->addTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
	assetManager->addTexture("radar-image", std::string("./assets/images/radar.png").c_str());
	assetManager->addTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
	assetManager->addTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
	assetManager->addFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);

	map = new Map("jungle-tiletexture", 2, 32);
	map->loadMap("./assets/tilemaps/jungle.map", 25, 20);

	player.addComponent<TransformComponent>(240, 106, 0 , 0, 32, 32, 1);
	player.addComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
	player.addComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
	player.addComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

	Entity& tankEntity(manager.addEntity("tank", ENEMY_LAYER));
	tankEntity.addComponent<TransformComponent>(150, 495, 25, 0, 32, 32, 1);
	tankEntity.addComponent<SpriteComponent>("tank-image");
	tankEntity.addComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

	Entity& heliport(manager.addEntity("heliport", OBSTACLE_LAYER));
	heliport.addComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
	heliport.addComponent<SpriteComponent>("heliport-image");
	heliport.addComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);
	
	Entity& radarEntity(manager.addEntity("radar", UI_LAYER));
	radarEntity.addComponent<TransformComponent>(720, 15, 0 , 0, 64, 64, 1);
	radarEntity.addComponent<SpriteComponent>("radar-image", 8, 150, false, true);

	Entity& labelLevelName(manager.addEntity("labelLevelName", UI_LAYER));
	labelLevelName.addComponent<TextLabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);
}

void Game::processInput() {
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

	handleCameraMovement();

	checkCollisions();
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

void Game::handleCameraMovement() {
	TransformComponent* mainPlayerTransform = player.getComponent<TransformComponent>();

	camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
	camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

	camera.x = camera.x < 0 ? 0 : camera.x;
	camera.y = camera.y < 0 ? 0 : camera.y;
	camera.x = camera.x > camera.w ? camera.w : camera.x;
	camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::checkCollisions() {
	CollisionType collisionType = manager.checkCollisions();

	if (collisionType == PLAYER_ENEMY_COLLISION) {
		processGameOver();
	}

	if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
		processNextLevel(1);
	}
}

void Game::processNextLevel(int levelNumber) {
	std::cout << "Next Level" << std::endl;
	isRunning = false;
}

void Game::processGameOver() {
	std::cout << "Game Over" << std::endl;
	isRunning = false;
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
