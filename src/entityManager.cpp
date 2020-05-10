#include <iostream>
#include "./entityManager.h"
#include "./collision.h"
#include "./components/colliderComponent.h"

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

CollisionType EntityManager::checkCollisions() const {
	for (int i = 0; i < entities.size() - 1; i++) {
		auto& thisEntity = entities[i];
		if (thisEntity->hasComponent<ColliderComponent>()) {
			ColliderComponent* thisCollider = thisEntity->getComponent<ColliderComponent>();
			for (int j = i + 1; j < entities.size(); j++) {
				auto& otherEntity = entities[j];
				if (thisEntity->name.compare(otherEntity->name) != 0 && otherEntity->hasComponent<ColliderComponent>()) {
					ColliderComponent* otherCollider = otherEntity->getComponent<ColliderComponent>();
					if (Collision::checkRectangleCollision(thisCollider->collider, otherCollider->collider)) {
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && otherCollider->colliderTag.compare("ENEMY") == 0) {
							return PLAYER_ENEMY_COLLISION;
						}
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && otherCollider->colliderTag.compare("PROJECTILE") == 0) {
							return PLAYER_PROJECTILE_COLLISION;
						}
						if (thisCollider->colliderTag.compare("ENEMY") == 0 && otherCollider->colliderTag.compare("PROJECTILE") == 0) {
							return ENEMY_PROJECTILE_COLLISION;
						}
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && otherCollider->colliderTag.compare("LEVEL_COMPLETE") == 0) {
							return PLAYER_LEVEL_COMPLETE_COLLISION;
						}
					}
				}
			}
			for (auto& otherEntity: entities) {
			}
		}
	}
	return NO_COLLISION;
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
