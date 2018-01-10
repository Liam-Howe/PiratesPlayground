#pragma once
#include "Component.h"

class ControlComponent : public BaseComponent<ControlComponent>
{
public:
	ControlComponent(int entityID, int id) 
	:
	 BaseComponent(entityID),
	 m_ID(id) 
	,m_shouldTakeMovementInput(true)
	{}

	enum Buttons
	{
		X,
		Y,
		B,
		RB,
		LB,
		LS,
		RS,
		A
	};
	
	int m_ID;
	bool m_shouldTakeMovementInput;
};
