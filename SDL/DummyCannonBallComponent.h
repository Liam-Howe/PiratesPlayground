#pragma once
#pragma once
#include "Component.h"
#include <vector>
#include "Entity.h"
#include "CannonBallComponent.h"
#include "Constants.h"
#include "SpriteComponent.h"
class DummyCannonBallComponent : public BaseComponent<DummyCannonBallComponent>
{
public:

	DummyCannonBallComponent::DummyCannonBallComponent() 
	{
		m_cannonBalls = std::vector<Entity*>();
	}

	std::vector<Entity*> m_cannonBalls;


	void addEntity(Entity * entity)
	{
		m_cannonBalls.push_back(entity);
	}


	void drop(void * tuple, EventHandlerComponent * eventHandler)
	{
		std::tuple<Vector2, int> m_tuple = *static_cast<std::tuple<Vector2, int>*>(tuple);
		
		Vector2 pos = std::get<0>(m_tuple);
		int id = std::get<1>(m_tuple);

		for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
		{
			if (m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_occupied == true && m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_playerID == id)
			{
				AudioManager::Instance()->playSound(SoundEffectID::DropCannonBall, 0);
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_occupied = false;
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_playerID = -1;
				m_cannonBalls[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y += Constants::playerSize.y/9;
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_timer = SDL_GetTicks();
				std::tuple<int> * myTuple = new std::tuple<int>{ id };
				Event * recieveCannonBall = new Event(Event::EventType::PlayerDropCannonBall, myTuple);
				EventHandlerSystem::AddEvent(recieveCannonBall, eventHandler);
			}
		}
	}




	void loadCannon(void* tuple)
	{

		std::tuple<int, EventHandlerComponent*> m_tuple = *static_cast<std::tuple<int, EventHandlerComponent*>*>(tuple);
		int id = std::get<0>(m_tuple);
		
		for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
		{
		
				if (m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_playerID == id)
				{
				    m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_occupied = false;
					
					std::tuple<int> * myTuple = new std::tuple<int>{ id };
					Event * recieveCannonBall = new Event(Event::EventType::PlayerDropCannonBall, myTuple);
					EventHandlerSystem::AddEvent(recieveCannonBall, std::get<1>(m_tuple));
				}
		}
	}


	void pickUp(void * tuple, EventHandlerComponent * eventHandler)
	{

		std::tuple<Vector2,int> m_tuple = *static_cast<std::tuple<Vector2,int>*>(tuple);

		Vector2 pos = std::get<0>(m_tuple);
		int id = std::get<1>(m_tuple);
		const SDL_Rect player{ pos.x,pos.y,Constants::playerSize.x - 15,Constants::playerSize.y };
		for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
		{
			if (m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_spawned && !m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_occupied)				
			{
				
				const SDL_Rect cannonballRect{ m_cannonBalls[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x ,m_cannonBalls[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y,Constants::cannonBallSize.x,Constants::cannonBallSize.y };
				
				bool value = SDL_HasIntersection(&player, &cannonballRect);
				if (value)
				{

					
					m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_occupied = true;
					m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_playerID = id;
					AudioManager::Instance()->playSound(SoundEffectID::Lift, 0);


					std::tuple<int> * myTuple = new std::tuple<int>{ id };
					Event * recieveCannonBall = new Event(Event::EventType::PlayerRecieveCannonBall, myTuple);
					EventHandlerSystem::AddEvent(recieveCannonBall, eventHandler);
					break;
				}
			}
			
		}
	}

	void spawn(EventHandlerComponent * eventHandler, void * tuple)
	{

		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(tuple);

		int id = std::get<0>(m_tuple);

		for (int i = 0; i <Constants::CANNON_BALL_SIZE; i++)
		{
			if (m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_spawned == false)
			{		
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_spawned = true;
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_occupied = true;
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_playerID = id;

				std::tuple<int> * myTuple = new std::tuple<int>{ id };
				Event * recieveCannonBall = new Event(Event::EventType::PlayerRecieveCannonBall, myTuple);
				EventHandlerSystem::AddEvent(recieveCannonBall, eventHandler);
				
				break;
			}	
		}
	}


	void fireCannonBall()
	{
		for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
		{
			if (!m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_fired)
			{
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_spawned = true;
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_fired = true;
				m_cannonBalls[i]->getComponent<CannonBallComponent>(ComponentTypes::CannonBall)->m_timer = SDL_GetTicks();
				break;
			}	
		}
	}
};
