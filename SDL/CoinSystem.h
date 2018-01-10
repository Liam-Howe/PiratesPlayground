#pragma once
#include "BaseSystem.h"
#include "CoinComponent.h"

class CoinSystem : public BaseSystem
{


public:
	void update(vector<Entity>& players, EventHandlerComponent* eventHandlerComponent,std::vector<SDL_Rect> goalRects)
	{
		for (int j = 0; j < players.size(); j++)
		{
			
			SDL_Rect playerRect = SDL_Rect{(int)players[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x,(int)players[j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y, 64,64};

			for (int i = 0; i < m_entities.size(); ++i)
			{
				
				CoinComponent* coinComponent = m_entities[i]->getComponent<CoinComponent>(ComponentTypes::CoinComponent);
				PositionComponent * coinposition = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);

				coinComponent->m_rect.x = coinposition->m_position.x;
				coinComponent->m_rect.y = coinposition->m_position.y;

				if (coinComponent->m_spawned ==true)
				{
					bool collide = SDL_HasIntersection(&coinComponent->m_rect, &playerRect);
					if (collide)
					{
						m_entities[i]->getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1].TimedTurnOn(0.3);
						m_entities[i]->getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOn();
						AudioManager::Instance()->playSound(SoundEffectID::Coin, 0);	
						coinComponent->m_spawned = false;
						coinComponent->m_spawnPosition  = coinposition->m_position;
 						std::tuple<int,int > * myTuple = new std::tuple<int,int >{ players[j].m_ID ,2};
						Event * startStationAnimation = new Event(Event::EventType::PickupCoin, myTuple);
						EventHandlerSystem::AddEvent(startStationAnimation, eventHandlerComponent);
						coinComponent->m_timer = SDL_GetTicks();
						
						if (players[j].m_ID == 1)
						{	
							m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel = (Vector2(goalRects[j].x, goalRects[j].y) - m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position).Normalize() * 0.2;
							if (players[j].getComponent<ControlComponent>(ComponentTypes::Control) != nullptr)
							{
								InputHandler::vibrate(players[j].getComponent<ControlComponent>(ComponentTypes::Control)->m_ID, 0.4, 250);
							}
						}
						else if (players[j].m_ID == 2)
						{
							if (players[j].getComponent<ControlComponent>(ComponentTypes::Control) != nullptr)
							{
								InputHandler::vibrate(players[j].getComponent<ControlComponent>(ComponentTypes::Control)->m_ID, 0.8, 250);
							}
							m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel = (Vector2(goalRects[j].x,goalRects[j].y) - m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position).Normalize() * 0.2;
							
						}	
					}
				}
				if (coinComponent->m_spawned == false)
				{
					 bool goalOnecollide = SDL_HasIntersection(&goalRects[j], &coinComponent->m_rect);		
					 if (goalOnecollide)
					 {
						 m_entities[i]->getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;
						 m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel = 0;
						 m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity = 0;

						 if (SDL_GetTicks() - coinComponent->m_timer > 10000)
						 {
							 coinComponent->m_spawned = true;
							 m_entities[i]->getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = true;
							
							 m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position = coinComponent->m_spawnPosition;
						 }
					 }
				}
			}
		}
	}
};


