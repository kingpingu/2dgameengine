#include "./assetManager.h"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {

}

void AssetManager::clearData() {
	textures.clear();
	fonts.clear();
}

void AssetManager::addTexture(std::string textureId, const char* filePath) {
	textures.emplace(textureId, TextureManager::loadTexture(filePath));
}

void AssetManager::addFont(std::string fontId, const char* filePath, int fontSize) {
	fonts.emplace(fontId, FontManager::loadFont(filePath, fontSize));
}

SDL_Texture* AssetManager::getTexture(std::string textureId) {
	return textures[textureId];
}

TTF_Font* AssetManager::getFont(std::string fontId) {
	return fonts[fontId];
}
