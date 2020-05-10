#include <iostream>
#include "./entity.h"

Entity::Entity(EntityManager& manager): manager(manager) {
	isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, LayerType layer): manager(manager), name(name), layer(layer) {
	isActive = true;
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
	isActive = false;
}

bool Entity::getIsActive() const {
	return isActive;
}

void Entity::listAllComponents() const {
	for (auto mapElement: componentTypeMap) {
		std::cout << "	Component<" << mapElement.first->name() << ">" << std::endl;
	}
}
