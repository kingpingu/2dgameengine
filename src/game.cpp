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
#include "./components/projectileEmitterComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* mainPlayer = NULL;
Map* map;

Game::Game() {
  this->isRunning = false;
}

Game::~Game() {

}

bool Game::getIsRunning() const {
  return this->isRunning;
}

void Game::initialize(int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL." << std::endl;
    return;
  }
  
	if (TTF_Init() != 0) {
    std::cerr << "Error initializing SDL TTF" << std::endl;
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

void Game::loadLevel(int levelNumber) {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

  std::string levelName = "level" + std::to_string(levelNumber);
  lua.script_file("./assets/scripts/" + levelName + ".lua");

	// load list of assets from lua config file
  sol::table levelData = lua[levelName];
  sol::table levelAssets = levelData["assets"];

  unsigned int assetIndex = 0;
  
	while (true) {
    sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
    
		if (existsAssetIndexNode == sol::nullopt) {
      break;
    } else {
      sol::table asset = levelAssets[assetIndex];
      std::string assetType = asset["type"];
      
			if (assetType.compare("texture") == 0) {
        std::string assetId = asset["id"];
        std::string assetFile = asset["file"];
        assetManager->addTexture(assetId, assetFile.c_str());
      }
      
			if (assetType.compare("font") == 0) {
        std::string assetId = asset["id"];
        std::string assetFile = asset["file"];
        int fontSize = asset["fontSize"];
        assetManager->addFont(assetId, assetFile.c_str(), fontSize);
      }
    }
    assetIndex++;
  }
	
	// load map from tile information from lua config file
  sol::table levelMap = levelData["map"];
  std::string mapTextureId = levelMap["textureAssetId"];
  std::string mapFile = levelMap["file"];

  map = new Map(
    mapTextureId,
    static_cast<int>(levelMap["scale"]),
    static_cast<int>(levelMap["tileSize"])
  );

  map->loadMap(
    mapFile,
    static_cast<int>(levelMap["mapSizeX"]),
    static_cast<int>(levelMap["mapSizeY"])
  );

	// load entities and components from lua config file
  sol::table levelEntities = levelData["entities"];
  
	unsigned int entityIndex = 0;
  
	while (true) {
    sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
    
		if (existsEntityIndexNode == sol::nullopt) {
      break;
    } else {
      sol::table entity = levelEntities[entityIndex];
      std::string entityName = entity["name"];
      LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

      // add new entity
      auto& newEntity(manager.addEntity(entityName, entityLayerType));

      // add Transform Component
      sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
      
			if (existsTransformComponent != sol::nullopt) {
        newEntity.addComponent<TransformComponent>(
          static_cast<int>(entity["components"]["transform"]["position"]["x"]),
          static_cast<int>(entity["components"]["transform"]["position"]["y"]),
          static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
          static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
          static_cast<int>(entity["components"]["transform"]["width"]),
          static_cast<int>(entity["components"]["transform"]["height"]),
          static_cast<int>(entity["components"]["transform"]["scale"])
        );
      }

      // add sprite component
      sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
      
			if (existsSpriteComponent != sol::nullopt) {
        std::string textureId = entity["components"]["sprite"]["textureAssetId"];
        bool isAnimated = entity["components"]["sprite"]["animated"];
        if (isAnimated) {
          newEntity.addComponent<SpriteComponent>(
            textureId,
            static_cast<int>(entity["components"]["sprite"]["frameCount"]),
            static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
            static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
            static_cast<bool>(entity["components"]["sprite"]["fixed"])
          );
        } else {
          newEntity.addComponent<SpriteComponent>(textureId);
        }
      }

      // add input control component
      sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
      
			if (existsInputComponent != sol::nullopt) {
        sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
        if (existsKeyboardInputComponent != sol::nullopt) {
          std::string upKey = entity["components"]["input"]["keyboard"]["up"];
          std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
          std::string downKey = entity["components"]["input"]["keyboard"]["down"];
          std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
          std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
          newEntity.addComponent<KeyboardControlComponent>(upKey, rightKey, downKey, leftKey, shootKey);
        }
      }

      // add collider component
      sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
      
			if (existsColliderComponent != sol::nullopt) {
        std::string colliderTag = entity["components"]["collider"]["tag"];
        newEntity.addComponent<ColliderComponent>(
          colliderTag,
          static_cast<int>(entity["components"]["transform"]["position"]["x"]),
          static_cast<int>(entity["components"]["transform"]["position"]["y"]),
          static_cast<int>(entity["components"]["transform"]["width"]),
          static_cast<int>(entity["components"]["transform"]["height"])
        );
      }

      // add projectile emitter component
      sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
      
			if (existsProjectileEmitterComponent != sol::nullopt) {
        int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
        int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
        int parentEntityWidth = entity["components"]["transform"]["width"];
        int parentEntityHeight = entity["components"]["transform"]["height"];
        int projectileWidth = entity["components"]["projectileEmitter"]["width"];
        int projectileHeight = entity["components"]["projectileEmitter"]["height"];
        int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
        int projectileRange = entity["components"]["projectileEmitter"]["range"];
        int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
        bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
        std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
        
				Entity& projectile(manager.addEntity("projectile", PROJECTILE_LAYER));
        
				projectile.addComponent<TransformComponent>(
          parentEntityXPos + (parentEntityWidth / 2),
          parentEntityYPos + (parentEntityHeight / 2),
          0,
          0,
          projectileWidth,
          projectileHeight,
          1
        );
        
				projectile.addComponent<SpriteComponent>(textureAssetId);
        
				projectile.addComponent<ProjectileEmitterComponent>(
          projectileSpeed,
          projectileAngle,
          projectileRange,
          projectileShouldLoop
        );
        
				projectile.addComponent<ColliderComponent>(
          "PROJECTILE",
          parentEntityXPos,
          parentEntityYPos,
          projectileWidth,
          projectileHeight
        );
      }
    }
    entityIndex++;
  }

  mainPlayer = manager.getEntityByName("player");
}

void Game::processInput() {
  SDL_PollEvent(&event);
  
	switch (event.type) {
    case SDL_QUIT: {
      isRunning = false;
      break;
    }
    
		case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        isRunning = false;
      }
    }
    
		default: {
      break;
    }
  }
}

void Game::update() {
  // wait until 16ms has ellapsed since the last frame
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	// while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

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
  if (mainPlayer) {
    TransformComponent* mainPlayerTransform = mainPlayer->getComponent<TransformComponent>();
    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
  }
}

void Game::checkCollisions() {
  CollisionType collisionType = manager.checkCollisions();
  
	if (collisionType == PLAYER_ENEMY_COLLISION) {
    processGameOver();
  }
  
	if (collisionType == PLAYER_PROJECTILE_COLLISION) {
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
