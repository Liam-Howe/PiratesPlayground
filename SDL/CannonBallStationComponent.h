#pragma once

#pragma once
#include "StationComponent.h"
#include "PositionComponent.h"

class CannonBallStationComponent : public StationComponent
{
public:
	CannonBallStationComponent(SDL_Point point,  Entity * stationEntity, PositionComponent * positionComponent,int cost, Vector2 glidePos) :
		StationComponent(point, stationEntity,cost, glidePos),
		m_boatComponent(positionComponent)
	{
		m_stationType = CannonBall;
	}


	PositionComponent * m_boatComponent;
};
