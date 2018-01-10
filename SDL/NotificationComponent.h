#pragma once
#include "Component.h"
#include "Vector2.h"
#include <tuple>
#include "ControlComponent.h"
#include "SpriteComponent.h"


class NotificationComponent : public BaseComponent<NotificationComponent>
{
public:
	PositionComponent * m_posComponent;
	SpriteComponent * m_spriteComponent;
	int m_entityID;

	int m_frameCounter;
	const int m_TOTAL_WHEEL_FRAMES = 5;

	ControlComponent::Buttons m_textureType;

	NotificationComponent(PositionComponent * position_component, SpriteComponent * sprite_component, int entityid)
		:m_posComponent(position_component),
		 m_spriteComponent(sprite_component),
		 m_textureType(ControlComponent::A),
		 m_entityID(entityid),
		 m_frameCounter(0)
	{
		
	}
	void setState(void* tuple)
	{
		std::tuple<Vector2, int> m_tuple = *static_cast<std::tuple<Vector2, int>*>(tuple);
		Vector2 position = std::get<0>(m_tuple);
		int id = std::get<1>(m_tuple);

		if(id == m_entityID)
		{
			Vector2 offset = { 30,-50 };
			m_posComponent->m_position = position + offset;
			m_spriteComponent->m_isAlive = true;
		}
	}

	void hide(void* tuple)
	{
		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(tuple);
		int id = std::get<0>(m_tuple);
		if(id == m_entityID)
		{
			m_spriteComponent->m_isAlive = false;
		}
	}

	void changeTexture(void * textureTuple)
	{
		std::tuple<ControlComponent::Buttons, int> tuple = *static_cast<std::tuple<ControlComponent::Buttons, int>*>(textureTuple);
		int id = std::get<1>(tuple);
		if (id == m_entityID)
		{
			m_textureType = std::get<0>(tuple);

			switch (m_textureType)
			{
			case ControlComponent::A:
				m_spriteComponent->m_sourceRect = { 0,0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;

			case ControlComponent::X:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * 2, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;

			case ControlComponent::B:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;

			case ControlComponent::Y:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * 3, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;
			case ControlComponent::RB:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * 5, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;
			case ControlComponent::LB:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * 4, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;
			case ControlComponent::LS:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * 6, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;
			case ControlComponent::RS:
				m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * 7, 0, m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };
				break;
			default:
				break;
			}
		}
	}

	void setNextNotificationAnim(void * idTuple)
	{
		std::tuple<int> tuple = *static_cast<std::tuple<int>*>(idTuple);
		int id = std::get<0>(tuple);
		if (id == m_entityID)
		{
			m_frameCounter++;
			if (m_frameCounter > m_TOTAL_WHEEL_FRAMES)
				m_frameCounter = 0;


			m_spriteComponent->m_sourceRect = { m_spriteComponent->m_sourceRect.w * m_frameCounter, m_spriteComponent->m_sourceRect.h,  m_spriteComponent->m_sourceRect.w, m_spriteComponent->m_sourceRect.h };

		}
	}
};
