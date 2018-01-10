#pragma once
#include "Component.h"


class AliveComponent : public BaseComponent<AliveComponent>
{
public:
	AliveComponent(int entityID)
	{
		m_enabled = true;
	}
	bool m_enabled;

};