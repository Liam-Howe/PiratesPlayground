#pragma once
#include "Entity.h"
#include "BaseSystem.h"
#include "CameraComponent.h"
#include "WorldRenderableComponent.h"
#include "PositionComponent.h"


class CameraSystem : public BaseSystem
{
public:
	int m_boatIndex;
	int m_boatShadowIndex;


	void setBoatIndex(int boatIndex)
	{
		for (int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities.at(i)->m_ID == boatIndex)
			{
				m_boatIndex = i;
			}
		}
	}
	void setBoatShadowIndex(int boatShadowIndex)
	{
		for (int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities.at(i)->m_ID == boatShadowIndex)
			{
				m_boatShadowIndex = i;
			}
		}
	}

	void Update(Entity * camera)
	{
		PositionComponent * entityPosComp = m_entities.at(m_boatIndex)->getComponent<PositionComponent>(ComponentTypes::Position);
		PositionComponent * camPosComp = camera->getComponent<PositionComponent>(ComponentTypes::Position);

		camPosComp->m_position.y = entityPosComp->m_position.y - (camera->getComponent<CameraComponent>(ComponentTypes::Camera)->SCREEN_SIZE.y * 1.2f);
	}


	void SetScreenPosition(Entity * camera)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			PositionComponent * entitySpatComp = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position);

			WorldRenderableComponent * wr = m_entities.at(i)->getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable);

			if(i != m_boatIndex && i != m_boatShadowIndex)
				wr->m_screenPos = WorldToScreen(camera, entitySpatComp->m_position);
			else
			{
				std::vector<float> * prevPosList = &m_entities[i]->getComponent<BoatComponent>(ComponentTypes::Boat)->m_prevPosList;

				SDL_Point newScreenPos = WorldToScreen(camera, entitySpatComp->m_position);

				if (prevPosList->size() < 30)
				{
					prevPosList->push_back(newScreenPos.y);
					wr->m_screenPos = newScreenPos;
				}
				else
				{
					prevPosList->insert(prevPosList->begin(),newScreenPos.y);
					prevPosList->pop_back();
				}
				
				float total = 0;

				for (int i = 0; i < prevPosList->size(); ++i)
				{
					total += prevPosList->at(i);
				}

				SDL_Point average{ 0,0 };

				average.x = newScreenPos.x;
				average.y = total / prevPosList->size();

				wr->m_screenPos = average;
			}
		}
	}

	static SDL_Point WorldToScreen(Entity * camera, Vector2 worldPosition)
	{
		PositionComponent * camPosComp = camera->getComponent<PositionComponent>(ComponentTypes::Position);
		SDL_Point screenPosition;

		screenPosition = { static_cast<int> (worldPosition.x - camPosComp->m_position.x) + Constants::MIRROR_X_POS, 
						   static_cast<int>( worldPosition.y - camPosComp->m_position.y)};

		return screenPosition;
	}

};
