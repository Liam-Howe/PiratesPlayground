#pragma once
#include "Component.h"
#include <SDL.h>
#include "Entity.h"
#include "Vector2.h"

class StationComponent : public BaseComponent<StationComponent>
{
public:
	StationComponent(SDL_Point point, Entity * stationEntity, int cost, Vector2 glidePos) :
		m_gridCoord(point),
		m_isActive(false),
		m_EntityUsingID(-1),
		m_stationEntity(stationEntity),
		m_isAIAssigned(false),
		m_cost(cost),
		m_glidePos(glidePos)
	{
		
	}

	enum Type
	{
		Wheel,
		Cannon,
		CannonBall,
		Engine,
		Furnace,
		Rudder
	};

	SDL_Point m_gridCoord;
	bool m_isActive;
	Type m_stationType;
	//the entityID of the user of this station
	int m_EntityUsingID;
	Entity * m_stationEntity;
	bool m_isAIAssigned;
	int m_cost;
	Vector2 m_glidePos;
};
