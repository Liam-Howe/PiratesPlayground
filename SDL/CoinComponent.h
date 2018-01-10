#pragma once
#include "Component.h"
#include "Entity.h"
#include "SDL.h"
class CoinComponent : public BaseComponent<CoinComponent>
{
public:

	CoinComponent(int id, SDL_Rect rect)
		: BaseComponent(id)
		, m_rect(rect)
		
	{
		m_spawned = true;
	
	}

	SDL_Rect m_rect;
	Vector2 m_spawnPosition;
	bool m_spawned;
	float m_timer;
};