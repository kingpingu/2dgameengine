#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../textureManager.h"
#include "../assetManager.h"
#include "./transformComponent.h"

class SpriteComponent: public Component {
	private:
		TransformComponent* transform;
		SDL_Texture* texture;
		SDL_Rect sourceRectangle;
		SDL_Rect destinationRectangle;
	public:
		SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
		
		SpriteComponent(const char* filePath) {
			setTexture(filePath);
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
			destinationRectangle.x = (int) transform->position.x;
			destinationRectangle.y = (int) transform->position.y;
			destinationRectangle.w = transform->width * transform->scale;
			destinationRectangle.h = transform->height * transform->scale;
		}

		void render() override {
			TextureManager::draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
		}
};

#endif
