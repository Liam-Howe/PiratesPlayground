#pragma once
#include "Component.h"
#include "StationComponent.h"

class TileComponent : public BaseComponent<TileComponent>
{
public:
	TileComponent(int type, bool collidable) :
	m_tileType(type),
	m_collidable(collidable)
	{
		
	}

	int m_tileType;
	StationComponent * m_stationComponent;
	bool m_collidable;
};