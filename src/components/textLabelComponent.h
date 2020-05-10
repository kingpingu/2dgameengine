#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../fontManager.h"
#include "../entityManager.h"
#include "../assetManager.h"
#include "../game.h"

class TextLabelComponent: public Component {
	private:
		SDL_Rect position;
		std::string text;
		std::string fontFamily;
		SDL_Color color;
		SDL_Texture* texture;
	
	public:
		TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color) {
			position.x = x;
			position.y = y;
			this->text = text;
			this->fontFamily = fontFamily;
			this->color = color;
			setLabelText(text, fontFamily);
		}

		void setLabelText(std::string text, std::string fontFamily) {
			SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->getFont(fontFamily), text.c_str(), color);
			texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
			SDL_FreeSurface(surface);
			SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
		}

		void render() override {
			FontManager::draw(texture, position);
		}
};

#endif
