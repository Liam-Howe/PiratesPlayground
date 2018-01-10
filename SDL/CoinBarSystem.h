#pragma once
#include "BaseSystem.h"
#include "CoinBarComponent.h"
#include "PositionComponent.h"
class CoinBarSystem : public BaseSystem
{


public:
	void update(vector<Entity>& players, vector<Entity>& barBackgrounds)
	{
		for (int j = 0; j < Constants::MAX_PLAYERS_PER_SHIP; j++)
		{
			for (int i = 0; i < m_entities.size(); i++)
			{
				CoinBarComponent * coinBarComponent = m_entities[i]->getComponent<CoinBarComponent>(ComponentTypes::CoinBarComponent);

				PositionComponent * positionComponent = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);
				SpriteComponent * spriteComponent = m_entities[i]->getComponent<SpriteComponent>(ComponentTypes::Sprite);

				if (coinBarComponent->m_playerId == players[j].m_ID)
				{
					positionComponent->m_position.x = players[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x - 15;
					positionComponent->m_position.y = players[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y - 25;
					barBackgrounds[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x = players[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x - 20;
					barBackgrounds[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y = players[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y - 30;
				}

				if (players[j].m_ID == coinBarComponent->m_playerId && players[j].getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_coinTally < Constants::MAX_COIN_VALUE
					&& players[j].getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_coinTally >= 0)
				{
					spriteComponent->m_sourceRect.w = players[j].getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_coinTally * 2;
					spriteComponent->m_size.x = players[j].getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_coinTally * 2;
				}
			}
		}	
	}
};

