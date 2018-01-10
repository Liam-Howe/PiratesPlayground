#pragma once
#include "Component.h"

class CannonBallComponent : public BaseComponent<CannonBallComponent>
{
public:
	

	CannonBallComponent(bool occupied) :
		m_occupied(occupied)
	{
		m_fired = false;
		m_spawned = false;
		m_timer = 0;
		m_speed = 1000;
	}


	bool m_occupied;
	bool m_fired;
	bool m_spawned;
	int m_timer;
	int m_playerID;
	float m_speed;
	
};
