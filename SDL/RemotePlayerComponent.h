#pragma once
#include "Component.h"

class RemotePlayerComponent : public BaseComponent<RemotePlayerComponent>
{
public:
	RemotePlayerComponent(int id)
		: m_networkID(id)
	{};
	int m_networkID;
};
