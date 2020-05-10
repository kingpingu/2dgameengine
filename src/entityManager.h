#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "./entity.h"
#include "./component.h"
#include <vector>

class EntityManager {
	private:
		std::vector<Entity*> entities;

	public:
		void clearData();
		void update(float deltaTime);
		void render();
		bool hasNoEntities() const;
		Entity& addEntity(std::string entityName);
		std::vector<Entity*> getEntities() const;
		std::vector<Entity*> getEntitiesByLayer(LayerType layer) const;
		unsigned int getEntitiesCount() const;
		void listAllEntities() const;
		Entity& addEntity(std::string entityName, LayerType layer);
		CollisionType checkCollisions() const;
		Entity* getEntityByName(std::string entityName);
};

#endif
