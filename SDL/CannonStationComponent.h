#pragma once
#include "StationComponent.h"
#include "PositionComponent.h"

class CannonStationComponent : public StationComponent
{
public:
	CannonStationComponent(SDL_Point point, Entity * stationEntity, PositionComponent * positionComponent,int cost, Vector2 glidePos) :
		StationComponent(point, stationEntity,cost, glidePos),
		m_boatComponent(positionComponent)
	{
		m_stationType = Cannon;
		m_loaded = false;
		
	}

	bool m_loaded;
	PositionComponent * m_boatComponent;




	void loadCannon(void * m_Tuple)
	{
		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(m_Tuple);
		int id = std::get<0>(m_tuple);
		if (id == m_stationEntity->m_ID)
		{
			m_loaded = true;
		}
	}

	void fireCannon(EventHandlerComponent * eventHandlerComp, void * m_Tuple)
	{
		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(m_Tuple);
		int id = std::get<0>(m_tuple);
		if (id == m_stationEntity->m_ID)
		{
			m_loaded = false;
			Event * fireCannonBall = new Event(Event::EventType::FireCannonBall, NULL);
			EventHandlerSystem::AddEvent(fireCannonBall, eventHandlerComp);
		}

		
	}

};
