#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../entityManager.h"
#include "../../lib/glm/glm.hpp"
#include <SDL2/SDL.h>
#include "../game.h"

class TransformComponent: public Component {
	public:
		glm::vec2 position;
		glm::vec2 velocity;
		int width;
		int height;
		int scale;
		
		TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
			position = glm::vec2(posX, posY);
			velocity = glm::vec2(velX, velY);
			width = w;
			height = h;
			scale = s;
		}
		
		void initialize() override {}
		
		void update(float deltaTime) override {
			position.x += velocity.x * deltaTime;
			position.y += velocity.y * deltaTime;
		}

		void render() override {
			SDL_Rect transformRectangle = { 
				(int) position.x,
				(int) position.y,
				width,
				height
			};
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(Game::renderer, &transformRectangle);
		}
};

#endif