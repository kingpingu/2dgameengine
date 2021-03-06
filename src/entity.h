#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include "./constants.h"
#include "./entityManager.h"
#include "./component.h"

class EntityManager;
class Component;

class Entity {
	private:
		EntityManager& manager;
		bool isActive;
		std::vector<Component*> components;
		std::map<const std::type_info*, Component*> componentTypeMap;

	public:
		std::string name;
		LayerType layer;
		Entity(EntityManager& manager);
		Entity(EntityManager& manager, std::string name, LayerType layer);
		void update(float deltaTime);
		void render();
		void destroy();
		bool getIsActive() const;
		void listAllComponents() const;

		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... args) {
			T* newComponent(new T(std::forward<TArgs>(args)...));
			newComponent->owner = this;
			components.emplace_back(newComponent);
			componentTypeMap[&typeid(*newComponent)] = newComponent;
			newComponent->initialize();
			return *newComponent;
		}

		template <typename T>
		T* getComponent() {
			return static_cast<T*>(componentTypeMap[&typeid(T)]);
		}

		template<typename T>
		bool hasComponent() const {
			return componentTypeMap.count(&typeid(T));
		}
};

#endif
