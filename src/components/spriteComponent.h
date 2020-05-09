#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../textureManager.h"
#include "../assetManager.h"
#include "./transformComponent.h"
#include "../animation.h"

class SpriteComponent: public Component {
	private:
		TransformComponent* transform;
		SDL_Texture* texture;
		SDL_Rect sourceRectangle;
		SDL_Rect destinationRectangle;
		bool isAnimated;
		int numFrames;
		int animationSpeed;
		bool isFixed;
		std::map<std::string, Animation> animations;
		std::string currentAnimationName;
		unsigned int animationIndex = 0;

		public:
		SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

		SpriteComponent(std::string assetTextureId) {
			isAnimated = false;
			isFixed = false;
			setTexture(assetTextureId);
		}

		SpriteComponent(std::string id, int numFrames, int animationSpeed, bool hasDirections, bool isFixed) {
			isAnimated = true;
			this->numFrames = numFrames;
			this->animationSpeed = animationSpeed;
			this->isFixed = isFixed;

			if (hasDirections) {
				Animation downAnimation = Animation(0, numFrames, animationSpeed);
				Animation rightAnimation = Animation(1, numFrames, animationSpeed);
				Animation leftAnimation = Animation(2, numFrames, animationSpeed);
				Animation upAnimation = Animation(3, numFrames, animationSpeed);

				animations.emplace("downAnimation", downAnimation);
				animations.emplace("rightAnimation", rightAnimation);
				animations.emplace("leftAnimation", leftAnimation);
				animations.emplace("upAnimation", upAnimation);

				animationIndex = 0;
				currentAnimationName = "downAnimation";
			} else {
				Animation singleAnimation = Animation(0, numFrames, animationSpeed);
				animations.emplace("singleAnimation", singleAnimation);
				animationIndex = 0;
				currentAnimationName = "singleAnimation";
			}

			play(currentAnimationName);

			setTexture(id);
		}

		void play(std::string animationName) {
			numFrames = animations[animationName].numFrames;
			animationIndex = animations[animationName].index;
			animationSpeed = animations[animationName].animationSpeed;
			currentAnimationName = animationName;
		}

		void setTexture(std::string assetTextureId) {
			texture = Game::assetManager->getTexture(assetTextureId);
		}

		void initialize() override {
			transform = owner->getComponent<TransformComponent>();
			sourceRectangle.x = 0;
			sourceRectangle.y = 0;
			sourceRectangle.w = transform->width;
			sourceRectangle.h = transform->height;
		}

		void update(float deltaTime) override {
			if (isAnimated) {
				sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
			}
			sourceRectangle.y = animationIndex * transform->height;

			destinationRectangle.x = static_cast<int>(transform->position.x);
			destinationRectangle.y = static_cast<int>(transform->position.y);
			destinationRectangle.w = transform->width * transform->scale;
			destinationRectangle.h = transform->height * transform->scale;
		}

		void render() override {
			TextureManager::draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
		}
};

#endif
