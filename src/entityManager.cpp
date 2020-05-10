#include <iostream>
#include "./entityManager.h"

void EntityManager::clearData() {
	for (auto& entity: entities) {
		entity->destroy();
	}
}

bool EntityManager::hasNoEntities() const {
	return entities.size() == 0;
}

void EntityManager::update(float deltaTime) {
	for (auto& entity: entities) {
		entity->update(deltaTime);
	}
}

void EntityManager::render() {
	for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
		for (auto& entity: getEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
			entity->render();
		}
	}
}

Entity& EntityManager::addEntity(std::string entityName, LayerType layer) {
	Entity *entity = new Entity(*this, entityName, layer);
	entities.emplace_back(entity);
	return *entity;
}

std::vector<Entity*> EntityManager::getEntities() const {
	return entities;
}

std::vector<Entity*> EntityManager::getEntitiesByLayer(LayerType layer) const {
	std::vector<Entity*> selectedEntities;
	
	for (auto& entity: entities) {
		if (entity->layer == layer) {
			selectedEntities.emplace_back(entity);
		}
	}

	return selectedEntities;
}

unsigned int EntityManager::getEntitiesCount() const {
	return entities.size();
}

void EntityManager::listAllEntities() const {
	unsigned int i = 0;
	for (auto& entity: entities) {
		std::cout << "Entity[" << i << "]: " << entity->name << std::endl;
		entity->listAllComponents();
		i++;
	}
}
