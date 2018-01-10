#pragma once
#include "Component.h"
#include  <tuple>

class SpriteComponent : public BaseComponent<SpriteComponent>
{
public:
	SpriteComponent(SDL_Texture * texture, SDL_Rect sourceRect , SDL_Point m_size, int entityID = -1, SDL_Point origin = {32,64})
	:	BaseComponent(entityID),
		m_texture(texture) ,
		m_sourceRect(sourceRect),
		m_size(m_size),
		m_isAlive(true)
	{
	}

	~SpriteComponent()
	{
		
	}

	SDL_Texture * m_texture;
	SDL_Rect m_sourceRect;
	SDL_Point m_size;
	bool m_isAlive;

	void setSourceRect(void * srcRect)
	{
		std::tuple<int, int, int> m_tuple = *static_cast<std::tuple<int, int, int>*>(srcRect);
		if (std::get<2>(m_tuple) == m_entityID)
		{
			m_sourceRect.x = std::get<0>(m_tuple);
			m_sourceRect.y = std::get<1>(m_tuple);
		}

	}
};
