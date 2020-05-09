#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "./textureManager.h"
#include "./entityManager.h"

class AssetManager {
	private:
		EntityManager* manager;
		std::map<std::string, SDL_Texture*> textures;
	public:
		AssetManager(EntityManager* manager);
		~AssetManager();
		void clearData();
		void addTexture(std::string textureId, const char* filePath);
		SDL_Texture* getTexture(std::string textureId);
};

#endif
