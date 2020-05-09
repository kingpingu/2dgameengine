#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../game.h"
#include "../entityManager.h"
#include "./transformComponent.h"
#include "./spriteComponent.h"

class KeyboardControlComponent: public Component {
	public:
		std::string upKey;
		std::string rightKey;
		std::string downKey;
		std::string leftKey;
		std::string shootKey;
		TransformComponent *transform;
		SpriteComponent *sprite;

		KeyboardControlComponent() {
		}

		KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey) {
			this->upKey = getSDLKeyStringCode(upKey);
			this->rightKey = getSDLKeyStringCode(rightKey);
			this->downKey = getSDLKeyStringCode(downKey);
			this->leftKey = getSDLKeyStringCode(leftKey);
			this->shootKey = getSDLKeyStringCode(shootKey);
		}

		std::string getSDLKeyStringCode(std::string key) {
			if (key.compare("up") == 0) return "1073741906";	
			if (key.compare("right") == 0) return "1073741903";
			if (key.compare("down") == 0) return "1073741905";
			if (key.compare("left") == 0) return "1073741904";
			if (key.compare("shoot") == 0) return "32";

			return std::to_string(static_cast<int>(key[0]));
		}

		void initialize() override {
			transform = owner->getComponent<TransformComponent>();
			sprite = owner->getComponent<SpriteComponent>();
		}

		void update(float deltaTime) override {
			if (Game::event.type == SDL_KEYDOWN) {
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);
				
				if (keyCode.compare(upKey) == 0) {
					transform->velocity.y = -100;
					transform->velocity.x = 0;
					sprite->play("upAnimation");
				}

				if (keyCode.compare(rightKey) == 0) {
					transform->velocity.y = 0;
					transform->velocity.x = 100;
					sprite->play("rightAnimation");
				}

				if (keyCode.compare(downKey) == 0) {
					transform->velocity.y = 100;
					transform->velocity.x = 0;
					sprite->play("downAnimation");

				}

				if (keyCode.compare(leftKey) == 0) {
					transform->velocity.y = 0;
					transform->velocity.x = -100;
					sprite->play("leftAnimation");

				}
				
				if (keyCode.compare(shootKey) == 0) {

				}
			}
			
			if (Game::event.type == SDL_KEYUP) {
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);
				
				if (keyCode.compare(upKey) == 0) {
					transform->velocity.y = 0;
				}

				if (keyCode.compare(rightKey) == 0) {
					transform->velocity.x = 0;
				}

				if (keyCode.compare(downKey) == 0) {
					transform->velocity.y = 0;
				}

				if (keyCode.compare(leftKey) == 0) {
					transform->velocity.x = 0;
				}
				
				if (keyCode.compare(shootKey) == 0) {

				}
			}
		}
};

#endif
