#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Entity.h"

class BoatAIComponent : public BaseComponent<BoatAIComponent>
{
public:
	const float m_MAX_SPEED = 0.00001f;


	BoatAIComponent(int id, Entity * boat)
		: BaseComponent(id),
		  m_nextWaypoint(nullptr), 
		  m_boat(boat)
	{
	}


	Vector2 * m_nextWaypoint;
	Entity * m_boat;
};
