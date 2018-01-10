#pragma once
#include "Component.h"

class CollisionComponent : public BaseComponent<CollisionComponent>
{
public:
	CollisionComponent(SDL_Rect rect) :
		m_collisionBounds(rect) 
	{};


	SDL_Rect m_collisionBounds;
};
