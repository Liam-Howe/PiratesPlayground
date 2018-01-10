#pragma once
#include "Entity.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "BaseSystem.h"
#include "WorldRenderableComponent.h"
#include "CoinBarComponent.h"
class RenderSystem : public BaseSystem
{
public:
	void render(SDL_Renderer * renderer)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			PositionComponent * positionComponent = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position);

			SpriteComponent * spriteComp = m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite);
			WorldRenderableComponent * worldRenderable = m_entities.at(i)->getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable);
			SDL_Rect destRect;

			if (worldRenderable == nullptr)
			{
				destRect = { static_cast<int>(positionComponent->m_position.x), static_cast<int>(positionComponent->m_position.y), static_cast<int>(spriteComp->m_size.x), static_cast<int>(spriteComp->m_size.y) };
			}
			else// jay wants to change this
				destRect = { static_cast<int>(worldRenderable->m_screenPos.x), static_cast<int>(worldRenderable->m_screenPos.y), static_cast<int>(spriteComp->m_size.x), static_cast<int>(spriteComp->m_size.y) };

			//SDL_RenderCopy(renderer, spriteComp->m_texture, &spriteComp->m_sourceRect, &destRect);

			if (spriteComp->m_isAlive)
			{
				SDL_Point centre = { spriteComp->m_size.x * 0.5, spriteComp->m_size.y * 0.5 };

				SDL_RenderCopyEx(renderer, spriteComp->m_texture, &spriteComp->m_sourceRect, &destRect, positionComponent->m_eulerRotation, &centre, SDL_FLIP_NONE);
			}

		}
	}

	void renderImage(SDL_Renderer * renderer, SpriteComponent * spriteComp, WorldRenderableComponent * worldRenderable, PositionComponent * positionComponent)
	{
		SDL_Rect destRect;

		if (worldRenderable == nullptr)
		{
			destRect = { static_cast<int>(positionComponent->m_position.x), static_cast<int>(positionComponent->m_position.y), static_cast<int>(spriteComp->m_size.x), static_cast<int>(spriteComp->m_size.y) };
		}
		else// jay wants to change this
			destRect = { static_cast<int>(worldRenderable->m_screenPos.x), static_cast<int>(worldRenderable->m_screenPos.y), static_cast<int>(spriteComp->m_size.x), static_cast<int>(spriteComp->m_size.y) };

		//SDL_RenderCopy(renderer, spriteComp->m_texture, &spriteComp->m_sourceRect, &destRect);

		if (spriteComp->m_isAlive)
		{
			SDL_Point centre = { 32, 64 };

			SDL_RenderCopyEx(renderer, spriteComp->m_texture, &spriteComp->m_sourceRect, &destRect, positionComponent->m_eulerRotation, &centre, SDL_FLIP_NONE);
		}

	}
};
