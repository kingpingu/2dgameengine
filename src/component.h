#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
	public:
		Entity* owner;
		virtual ~Component() {}
		virtual void initialize() {}
		virtual void update(float deltaTime) {}
		virtual void render() {}
};

#endif
