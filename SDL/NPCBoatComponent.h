#pragma once
#include "Component.h"


class NPCBoatComponent : public BaseComponent<NPCBoatComponent>
{
public:
	NPCBoatComponent(int entityID)
		: BaseComponent<NPCBoatComponent>(entityID)
	{  }

};
