#pragma once
#include "Component.h"
#include "Vector2.h"

class PositionComponent : public BaseComponent<PositionComponent>
{
public:
	PositionComponent(Vector2 pos, int entityID = -1)
	:	BaseComponent<PositionComponent>(entityID)
	,   m_position(pos) 
	,	m_previousPosition(Vector2(0,0))
	,	m_eulerRotation(0)
	{
		
	}
	Vector2 m_position, m_previousPosition;
	float m_eulerRotation;

};

//