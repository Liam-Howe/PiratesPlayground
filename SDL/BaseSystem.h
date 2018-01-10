#pragma once
#include "Entity.h"

class BaseSystem
{
public:
	void addEntity(Entity * entity)
	{
		m_entities.push_back(entity);
	}
protected:
	std::vector<Entity *> m_entities;
};
