#pragma once
#include "InputHandler.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "CollisionComponent.h"
#include "TileComponent.h"
#include "CoinComponent.h"
#include "ParticleComponent.h"
class WorldCollisionSystem : public BaseSystem
{
public:
	int handleCollisions(std::vector<std::vector<Entity>> & tileEntities, EventHandlerComponent * eventHandlerComp)
	{
		int result = 0;
		
			PositionComponent * boatpositioncomponent = m_entities[0]->getComponent<PositionComponent>(ComponentTypes::Position);
			CollisionComponent * boatCollisionComponent = m_entities[0]->getComponent<CollisionComponent>(ComponentTypes::Collision);

			VelocityComponent * boatVelocityComponent = m_entities[0]->getComponent<VelocityComponent>(ComponentTypes::Velocity);
		
			if (boatpositioncomponent->m_previousPosition.x != boatpositioncomponent->m_position.x)
			{
				boatCollisionComponent->m_collisionBounds.x = boatpositioncomponent->m_position.x;
				boatCollisionComponent->m_collisionBounds.y = boatpositioncomponent->m_previousPosition.y;

				result = handleCollisionAxis(Direction::Horizontal, tileEntities, boatpositioncomponent, boatCollisionComponent, boatVelocityComponent, eventHandlerComp);

			}
			if (boatpositioncomponent->m_previousPosition.y != boatpositioncomponent->m_position.y)
			{
				boatCollisionComponent->m_collisionBounds.x = boatpositioncomponent->m_position.x ;
				boatCollisionComponent->m_collisionBounds.y = boatpositioncomponent->m_position.y;				// Set CollisionBounds X Position
				
				result = handleCollisionAxis(Direction::Vertical, tileEntities, boatpositioncomponent, boatCollisionComponent, boatVelocityComponent, eventHandlerComp);
			}
	
		return result;
	}

	int handleCollisionAxis(Direction direction, std::vector<std::vector<Entity>> & tileEntities, PositionComponent * boatPositionComponent, CollisionComponent * boatCollisionComponent,VelocityComponent * boatVelocityComponent, EventHandlerComponent * eventHandlerComp)
	{
		int result = 0;
		int leftTile = (boatCollisionComponent->m_collisionBounds.x) / Constants::TILE_SIZE;
		int topTile = boatCollisionComponent->m_collisionBounds.y / Constants::TILE_SIZE;
		int rightTile = (int)ceil((float)(boatCollisionComponent->m_collisionBounds.x + boatCollisionComponent->m_collisionBounds.w) / Constants::TILE_SIZE) - 1;
		int bottomTile = (int)ceil(((float)(boatCollisionComponent->m_collisionBounds.y + boatCollisionComponent->m_collisionBounds.h) / Constants::TILE_SIZE)) - 1;
		bool collide = false;
		for (int y = topTile; y <= bottomTile; ++y)
		{
			for (int x = leftTile; x <= rightTile; ++x)
			{
				if (direction == Direction::Horizontal)	// X Axis
				{
					TileComponent * tileC = tileEntities[y][x].getComponent<TileComponent>(ComponentTypes::Tile);
					CollisionComponent * tileCC = tileEntities[y][x].getComponent<CollisionComponent>(ComponentTypes::Collision);
					if (tileC->m_collidable)
					{
						
						if (abs(boatVelocityComponent->m_velocity.x) > 0)
						{
						
							std::tuple<int, float> * tuple = new std::tuple<int, float>{ m_entities[0]->m_ID, 0.05f };
							Event * playerDamage = new Event(Event::EventType::Damage, tuple);
							EventHandlerSystem::AddEvent(playerDamage, eventHandlerComp);
						}

						float depth = Constants::GetHorizontalIntersectionDepth(boatCollisionComponent->m_collisionBounds, tileCC->m_collisionBounds);
						
						if (depth < 0)
						{
							boatPositionComponent->m_position.x = tileCC->m_collisionBounds.x - Constants::TILE_SIZE;
						}
						else if (depth > 0)
						{
							boatPositionComponent->m_position.x = tileCC->m_collisionBounds.x + Constants::TILE_SIZE ;
						}
						boatPositionComponent->m_eulerRotation = 0;
						boatVelocityComponent->m_velocity.x = 0;
						boatCollisionComponent->m_collisionBounds.x = boatPositionComponent->m_position.x;
			
					}
					else if(tileC->m_tileType == 2)
					{
						result = 1;
					}
				}
				else if (direction == Direction::Vertical)	// Y Axis
				{
					TileComponent * tileC = tileEntities[y][x].getComponent<TileComponent>(ComponentTypes::Tile);
					CollisionComponent * tileCC = tileEntities[y][x].getComponent<CollisionComponent>(ComponentTypes::Collision);
					
					if (tileC->m_collidable)
					{
					
						float depth = Constants::GetVerticalIntersectionDepth(boatCollisionComponent->m_collisionBounds, tileCC->m_collisionBounds);
						if (depth < 0)
							boatPositionComponent->m_position.y = tileCC->m_collisionBounds.y - Constants::TILE_SIZE;
						else
							boatPositionComponent->m_position.y = tileCC->m_collisionBounds.y + Constants::TILE_SIZE;

						boatVelocityComponent->m_velocity.y = 0;
						boatVelocityComponent->m_accel = 0;
						
					}
					else if (tileC->m_tileType == 2)
					{	
						result = 1;
					}

				}
			}
		}
		return result;
	}

	void entityColision(std::vector<Entity> & entities, EventHandlerComponent * eventHandlerComponent)
	{
		for (int i = 0; i < m_entities.size(); i++)
		{
			PositionComponent * positioncomponent = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);
			CollisionComponent * collisionComponent = m_entities[i]->getComponent<CollisionComponent>(ComponentTypes::Collision);
			const SDL_Rect boatRect{ collisionComponent->m_collisionBounds.x,collisionComponent->m_collisionBounds.y,collisionComponent->m_collisionBounds.w,collisionComponent->m_collisionBounds.h };
			for (int j = 0; j < entities.size(); j++)
			{
				AliveComponent * entityAliveComponent = entities[j].getComponent<AliveComponent>(ComponentTypes::AliveComponent);
				if (entityAliveComponent->m_enabled)
				{
					CollisionComponent * entityCollisionComponent = entities[j].getComponent<CollisionComponent>(ComponentTypes::Collision);
					SpriteComponent * entitySpriteComponent = entities[j].getComponent<SpriteComponent>(ComponentTypes::Sprite);
					const	SDL_Rect entityRect{ entityCollisionComponent->m_collisionBounds.x,entityCollisionComponent->m_collisionBounds.y,entityCollisionComponent->m_collisionBounds.w,entityCollisionComponent->m_collisionBounds.h };
					bool collide = SDL_HasIntersection(&boatRect, &entityRect);
					
					if (collide)
					{		
						entitySpriteComponent->m_isAlive = false;
						entityAliveComponent->m_enabled = false;
						std::tuple<int, int> * myTuple = new std::tuple<int, int>{ -1,10 };
						Event * startStationAnimation = new Event(Event::EventType::SpendCoin, myTuple);
						EventHandlerSystem::AddEvent(startStationAnimation, eventHandlerComponent);
						entities[j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TimedTurnOn(0.5);
						std::tuple<int, float> * tuple = new std::tuple<int, float>{ m_entities[0]->m_ID, 0.1f };
						Event * playerDamage = new Event(Event::EventType::Damage, tuple);
						EventHandlerSystem::AddEvent(playerDamage, eventHandlerComponent);
						AudioManager::Instance()->playSound(SoundEffectID::Sploosh, 0);
					}
				}
			}
		}
	}
	
	void boatOnboatCollision()
	{
		for (int i = 0; i < m_entities.size(); i++)
		{
			for (int j = 0; j < m_entities.size(); j++)
			{

				if (i == j)
				{
					continue;
				}

				PositionComponent * positioncomponent = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);
				CollisionComponent * collisionComponent = m_entities[i]->getComponent<CollisionComponent>(ComponentTypes::Collision);
				VelocityComponent * veloctiyComponent = m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity);
				
				PositionComponent * enemyPositionComponent = m_entities[j]->getComponent<PositionComponent>(ComponentTypes::Position);
				CollisionComponent * enemyCollisionComponent = m_entities[j]->getComponent<CollisionComponent>(ComponentTypes::Collision);
				VelocityComponent * enemyVeloctiyComponent = m_entities[j]->getComponent<VelocityComponent>(ComponentTypes::Velocity);
				
				 SDL_Rect boatRect{ collisionComponent->m_collisionBounds.x,collisionComponent->m_collisionBounds.y,collisionComponent->m_collisionBounds.w,collisionComponent->m_collisionBounds.h };
				 SDL_Rect enemyBoatRect{ enemyCollisionComponent->m_collisionBounds.x,enemyCollisionComponent->m_collisionBounds.y,enemyCollisionComponent->m_collisionBounds.w,enemyCollisionComponent->m_collisionBounds.h };


				collisionComponent->m_collisionBounds.x = positioncomponent->m_position.x;
				collisionComponent->m_collisionBounds.y = positioncomponent->m_position.y;
				
				enemyCollisionComponent->m_collisionBounds.x = enemyPositionComponent->m_position.x;
				enemyCollisionComponent->m_collisionBounds.y = enemyPositionComponent->m_position.y;
			

				bool collide = SDL_HasIntersection(&boatRect, &enemyBoatRect);

				if (collide)
				{
					float xDepth = Constants::GetHorizontalIntersectionDepth(boatRect,enemyBoatRect);
					float yDepth = Constants::GetVerticalIntersectionDepth(boatRect, enemyBoatRect);
#

					if (abs(xDepth) < abs(yDepth))
					{
						positioncomponent->m_position.x += xDepth;
						collisionComponent->m_collisionBounds.x = positioncomponent->m_position.x;

					}
					else
					{
						positioncomponent->m_position.y += yDepth;
						collisionComponent->m_collisionBounds.y = positioncomponent->m_position.y;
					}

				}
			}			
		}
	}	
};



