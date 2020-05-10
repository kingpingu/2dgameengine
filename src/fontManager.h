#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "./game.h"

class FontManager {
	public:
		static TTF_Font* loadFont(const char* fileName, int fontSize);
		static void draw(SDL_Texture* texture, SDL_Rect position);
};

#endif
