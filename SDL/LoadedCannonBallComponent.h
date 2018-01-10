#pragma once
#include "Component.h"

class LoadedCannonBallComponent : public BaseComponent<LoadedCannonBallComponent>
{
public:


	LoadedCannonBallComponent(int entityID = -1) 
		: BaseComponent<LoadedCannonBallComponent>(entityID)
	{
		int m_cannonID = 0;
	}

	int m_cannonID;
	

};
