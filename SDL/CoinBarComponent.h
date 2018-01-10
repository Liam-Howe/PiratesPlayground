#pragma once
#include "Component.h"
#include "SDL.h"
class CoinBarComponent : public BaseComponent<CoinBarComponent>
{
public:
	CoinBarComponent(int playerID) :
		m_playerId(playerID)
	{

	};

	int m_playerId;
};
