#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "./game.h"

class TextureManager {
	public:
		static SDL_Texture* loadTexture(const char* fileName);
		static void draw(SDL_Texture* texture, SDL_Rect sourceRectangel, SDL_Rect destinationRectangle, SDL_RendererFlip flip);

};

#endif
