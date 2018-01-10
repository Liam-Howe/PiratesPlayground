#pragma once
#include "BaseSystem.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"

class EntityAnimationSystem : public BaseSystem
{
public:

	void update(float deltaTime)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			AnimationComponent * animationComp = m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation);
			SpriteComponent * spriteComponent = m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite);
			
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
			}
			else
			{
				animationComp->m_currentFrame = 0;
			}
			SDL_Rect temp{ animationComp->m_currentFrame * animationComp->m_frameSize.x, 0, animationComp->m_frameSize.x, animationComp->m_frameSize.y };
			spriteComponent->m_sourceRect = temp;
		}
	}

};
