#pragma once
#include "BaseSystem.h"
#include "AnimationComponent.h"
#include "EventHandlerComponent.h"
#include "EventHandlerSystem.h"

class AnimationSystem : public BaseSystem
{
public:

	void update(float deltaTime, EventHandlerComponent * eventHandlerComp)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			AnimationComponent * animationComp = m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation);
			if (animationComp->m_shouldUpdateAnimation)
			{
				animationComp->m_frameTimer += deltaTime * animationComp->m_speedScalar;
				if (animationComp->m_frameTimer > animationComp->m_timeBetweenFrames)
				{
					animationComp->m_frameTimer = 0;
					animationComp->m_currentFrame++;
					if (animationComp->m_currentFrame > animationComp->m_numOfFrames)
					{
						animationComp->m_currentFrame = 0;
					}
				}

				std::tuple<int, int, int> * myTuple = new std::tuple<int, int, int>{ int(animationComp->m_currentFrame * animationComp->m_frameSize.x) , int(animationComp->m_animationType * animationComp->m_frameSize.y), m_entities[i]->m_ID };
				Event * setSourceRect = new Event(Event::EventType::SetSourceRect, myTuple);
				EventHandlerSystem::AddEvent(setSourceRect, eventHandlerComp);	
			}
			else
			{
				animationComp->m_currentFrame = 0;

				std::tuple<int, int, int> * myTuple = new std::tuple<int, int, int>{ int(animationComp->m_currentFrame * animationComp->m_frameSize.x), int(animationComp->m_animationType * animationComp->m_frameSize.y), m_entities[i]->m_ID };
				Event * setSourceRect = new Event(Event::EventType::SetSourceRect, myTuple);
				EventHandlerSystem::AddEvent(setSourceRect, eventHandlerComp);
			}

		}
	}

};
