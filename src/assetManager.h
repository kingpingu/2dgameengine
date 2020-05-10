#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "./textureManager.h"
#include "./fontManager.h"
#include "./entityManager.h"

class AssetManager {
	private:
		EntityManager* manager;
		std::map<std::string, SDL_Texture*> textures;
		std::map<std::string, TTF_Font*> fonts;
	
	public:
		AssetManager(EntityManager* manager);
		~AssetManager();
		void clearData();
		void addTexture(std::string textureId, const char* filePath);
		void addFont(std::string fontId, const char* filePath, int fontSize);
		SDL_Texture* getTexture(std::string textureId);
		TTF_Font* getFont(std::string fontId);
};

#endif
