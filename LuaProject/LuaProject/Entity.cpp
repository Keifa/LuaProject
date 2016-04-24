#include "Entity.h"



Entity::Entity()
{
}

Entity::~Entity()
{
	for (auto& c : components) {
		delete c.second;
	}
}

void Entity::addComponent(std::type_index type, Component * c)
{
	components[type] = c;
}
