#pragma once
#include "StationComponent.h"
#include <SDL.h>

class RudderStation : public StationComponent
{
public:
	RudderStation(SDL_Point point, Entity * stationEntity,int cost, Vector2 glidePos):
		StationComponent(point, stationEntity, cost, glidePos),
		m_amountOfButtonPressed(0)
	{
		m_stationType = Rudder;
	}

	void incrementButtonPress()
	{
		m_amountOfButtonPressed++;
	}

	int m_amountOfButtonPressed;
};
