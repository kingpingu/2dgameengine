#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "./entity.h"
#include "./component.h"
#include <vector>
#include <iostream>

class EntityManager {
	private:
		std::vector<Entity*> entities;
	public:
		void clearData();
		void update(float deltaTime);
		void render();
		bool hasNoEntities();
		Entity& addEntity(std::string entityName);
		std::vector<Entity*> getEntities() const;
		unsigned int getEntitiesCount();
		void listAllEntities() const;
};

#endif
