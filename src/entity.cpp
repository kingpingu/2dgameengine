#include "./entity.h"

Entity::Entity(EntityManager& manager): manager(manager) {
	this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name): manager(manager), name(name) {
	this->isActive = true;
}

void Entity::update(float deltaTime) {
	for (auto& component: components) {
		component->update(deltaTime);
	}
}

void Entity::render() {
	for (auto& component: components) {
		component->render();
	}
}

void Entity::destroy() {
	this->isActive = false;
}

bool Entity::getIsActive() const {
	return this->isActive;
}
