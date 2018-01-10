#pragma once

#include "BaseSystem.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "CannonBallComponent.h"
#include "SpriteComponent.h"
#include "AliveComponent.h"

class CannonBallSystem : public BaseSystem
{
public:
	

	void updateRightHandSide(std::vector<Vector2*> playerposition, std::vector<Vector2> cannonPosition, EventHandlerComponent * eventHandlerComp, Vector2 initialposition)
	{
		for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
		{
	  
			CannonBallComponent * cannonBallComponent = m_entities.at(i)->getComponent<CannonBallComponent>(ComponentTypes::CannonBall);
			PositionComponent * positionComponent = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position);
			SpriteComponent * spriteComponent = m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite);

			if (cannonBallComponent->m_spawned && !cannonBallComponent->m_occupied)
			{
				spriteComponent->m_isAlive = true;
			}
			else if (cannonBallComponent->m_spawned && cannonBallComponent->m_occupied)
			{
				spriteComponent->m_isAlive = false;
			}


			if (cannonBallComponent->m_spawned && !cannonBallComponent->m_occupied)   
			{
				if (SDL_GetTicks() - cannonBallComponent->m_timer > 10000)
				{
					

					cannonBallComponent->m_spawned = false;
					spriteComponent->m_isAlive = false;
					cannonBallComponent->m_timer = 0;
					positionComponent->m_position = initialposition;
					cannonBallComponent->m_timer = 0;
				}

			}
			
		

			if (cannonBallComponent->m_spawned && cannonBallComponent->m_occupied && !cannonBallComponent->m_fired)
			{
				positionComponent->m_position = Vector2(playerposition.at(cannonBallComponent->m_playerID -1)->x + Constants::playerSize.x/4, playerposition.at(cannonBallComponent->m_playerID - 1)->y + Constants::playerSize.y/2);
				cannonBallComponent->m_timer = 0;
			}
		}
	}
	void updateLeftHandSide(Vector2 boatPos,float dt)
	{
		for (int i = Constants::CANNON_BALL_SIZE; i < m_entities.size(); i++)
		{
			CannonBallComponent * cannonBallComponent = m_entities.at(i)->getComponent<CannonBallComponent>(ComponentTypes::CannonBall);
			PositionComponent * positionComponent = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position);
			SpriteComponent * spriteComponent = m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite);
		
			if (cannonBallComponent->m_spawned)
			{
				positionComponent->m_position.x = boatPos.x - spriteComponent->m_size.x/2;
				positionComponent->m_position.y = boatPos.y;
				cannonBallComponent->m_spawned = false;
			}

			if (cannonBallComponent->m_fired)
			{	
				spriteComponent->m_isAlive = true;
				positionComponent->m_position.y -= cannonBallComponent->m_speed * dt;
				if (cannonBallComponent->m_fired && SDL_GetTicks() - cannonBallComponent->m_timer  > 3000)
				{	
					cannonBallComponent->m_fired = false;
					spriteComponent->m_isAlive = false;
					cannonBallComponent->m_timer = 0;
					positionComponent->m_position = boatPos;	
				
				}
			}
		}
	}
	void updateEntityCollsionBoxes(std::vector<Entity> & entities, Vector2 boatPos, std::vector<Entity*> & enemyBoats, std::vector<std::vector<Entity>> & tileEntities, EventHandlerComponent* eventHandlerComponent)
	{
		for (int i = Constants::CANNON_BALL_SIZE; i < m_entities.size(); i++)
		{
			CannonBallComponent * cannonBallComponent = m_entities[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall);
			if (cannonBallComponent->m_fired)
			{
				PositionComponent * positioncomponent = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);
				CollisionComponent * collisionComponent = m_entities[i]->getComponent<CollisionComponent>(ComponentTypes::Collision);

				SpriteComponent * spriteComponent = m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite);
				collisionComponent->m_collisionBounds.x = positioncomponent->m_position.x;
				collisionComponent->m_collisionBounds.y = positioncomponent->m_position.y;
				const SDL_Rect cannonRect{ collisionComponent->m_collisionBounds.x,collisionComponent->m_collisionBounds.y,collisionComponent->m_collisionBounds.w,collisionComponent->m_collisionBounds.h };
				updateEntityCollision(entities,cannonRect,cannonBallComponent,positioncomponent,spriteComponent,boatPos, eventHandlerComponent);
				updateBoatCollision(enemyBoats, cannonRect, cannonBallComponent, positioncomponent, spriteComponent, boatPos);
				updateTileCollision(tileEntities, cannonRect, cannonBallComponent, positioncomponent, spriteComponent, boatPos);
			}
		}
	}


	void updateEntityCollision(std::vector<Entity> & entities,const SDL_Rect cannonBallRect,CannonBallComponent * cannonBallComponent, PositionComponent *positioncomponent,SpriteComponent*spriteComponent,Vector2 boatPos, EventHandlerComponent* eventHandlerComponent)
	{
		for (int j = 0; j < entities.size(); j++)
		{
			AliveComponent * entityAliveComponent = entities[j].getComponent<AliveComponent>(ComponentTypes::AliveComponent);
			if (entityAliveComponent->m_enabled)
			{
				CollisionComponent * entityCollisionComponent = entities[j].getComponent<CollisionComponent>(ComponentTypes::Collision);
				SpriteComponent * entitySpriteComponent = entities[j].getComponent<SpriteComponent>(ComponentTypes::Sprite);
				const	SDL_Rect entityRect{ entityCollisionComponent->m_collisionBounds.x,entityCollisionComponent->m_collisionBounds.y,entityCollisionComponent->m_collisionBounds.w,entityCollisionComponent->m_collisionBounds.h };
				bool collide = SDL_HasIntersection(&cannonBallRect, &entityRect);
				if (collide)
				{
					entities[j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TimedTurnOn(0.5);
					std::tuple<int,int> * myTuple = new std::tuple<int,int>{-1,20};
					Event * startStationAnimation = new Event(Event::EventType::PickupCoin, myTuple);
					EventHandlerSystem::AddEvent(startStationAnimation, eventHandlerComponent);
					cannonBallComponent->m_fired = false;
					spriteComponent->m_isAlive = false;
					cannonBallComponent->m_timer = 0;
					positioncomponent->m_position = boatPos;
					entitySpriteComponent->m_isAlive = false;
					entityAliveComponent->m_enabled = false;
					AudioManager::Instance()->playSound(SoundEffectID::Kaboom, 0);
				}
			}
		}
	}

	void updateBoatCollision(std::vector<Entity*> & boatentities, const SDL_Rect cannonBallRect, CannonBallComponent * cannonBallComponent, PositionComponent *positioncomponent, SpriteComponent*spriteComponent, Vector2 boatPos)
	{
		if (cannonBallComponent->m_fired)
		{
			for (int j = 0; j < boatentities.size(); j++)
			{
				CollisionComponent * entityCollisionComponent = boatentities[j]->getComponent<CollisionComponent>(ComponentTypes::Collision);
				PositionComponent * entityPositionComponent = boatentities[j]->getComponent<PositionComponent>(ComponentTypes::Position);
				entityCollisionComponent->m_collisionBounds.x = entityPositionComponent->m_position.x;
				entityCollisionComponent->m_collisionBounds.y = entityPositionComponent->m_position.y;

				SpriteComponent * entitySpriteComponent = boatentities[j]->getComponent<SpriteComponent>(ComponentTypes::Sprite);
				const	SDL_Rect boatRect{ entityCollisionComponent->m_collisionBounds.x,entityCollisionComponent->m_collisionBounds.y,entityCollisionComponent->m_collisionBounds.w,entityCollisionComponent->m_collisionBounds.h };
				bool collide = SDL_HasIntersection(&cannonBallRect, &boatRect);
				if (collide)
				{
					cannonBallComponent->m_fired = false;
					spriteComponent->m_isAlive = false;
					cannonBallComponent->m_timer = 0;
					positioncomponent->m_position = boatPos;
					//entitySpriteComponent->m_isAlive = false;
					//dispatch boat damage message
				}
			}
		}
	}

	void updateTileCollision(std::vector<std::vector<Entity>> & tileEntities, const SDL_Rect cannonBallRect, CannonBallComponent * cannonBallComponent, PositionComponent *positioncomponent, SpriteComponent*spriteComponent, Vector2 boatPos)
	{
		int leftTile = (cannonBallRect.x) / Constants::TILE_SIZE;
		int topTile = cannonBallRect.y / Constants::TILE_SIZE;
		int rightTile = (int)ceil((float)(cannonBallRect.x + cannonBallRect.w) / Constants::TILE_SIZE) - 1;
		int bottomTile = (int)ceil(((float)(cannonBallRect.y + cannonBallRect.h) / Constants::TILE_SIZE)) - 1;


		for (int y = topTile; y <= bottomTile; ++y)
		{
			for (int x = leftTile; x <= rightTile; ++x)
			{
				TileComponent * tileC = tileEntities[y][x].getComponent<TileComponent>(ComponentTypes::Tile);
				if (tileC->m_collidable)
				{

					cannonBallComponent->m_fired = false;
					spriteComponent->m_isAlive = false;
					cannonBallComponent->m_timer = 0;
					positioncomponent->m_position = boatPos;
				}
			}
		}
	}
};