#pragma once
#include "StationComponent.h"
#include <SDL.h>


class FurnaceStationComponent : public StationComponent
{
public:

	float m_time;


	FurnaceStationComponent(SDL_Point point, Entity * stationEntity, int cost, Vector2 glidePos): StationComponent(point, stationEntity, cost, glidePos)
	{
		m_stationType = Furnace;
		m_buttonNeeded = X;
	}

	enum NextButtonNeeded
	{
		X,
		B
	};

	NextButtonNeeded m_buttonNeeded;

	void changeButtonNeeded()
	{
		if (m_buttonNeeded == X)
			m_buttonNeeded = B;
		else
			m_buttonNeeded = X;
	}

	void resetButtonNeeded()
	{
		m_buttonNeeded = X;
	}
};
