#pragma once
#include "Component.h"
#include "StationComponent.h"
#include "Vector2.h"
#include "Constants.h"

class AIControlComponent : public BaseComponent<AIControlComponent>
{
public:
	AIControlComponent(int entityID)
		: BaseComponent<AIControlComponent>(entityID),
		  m_readyToLeaveStation(true),
		  m_oldStation(nullptr),
		  m_buttonNeeded(true)
	{
	}


	StationComponent * m_stationOccupying;
	std::vector<Vector2> m_pathToStation;
	
	float m_timeSinceInput;
	float m_timeBeforeInput;
	bool m_readyToLeaveStation;
	StationComponent * m_oldStation;
	StationComponent * m_stationToAimFor;
	bool m_buttonNeeded;

};
