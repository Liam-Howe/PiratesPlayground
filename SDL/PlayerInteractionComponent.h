#pragma once
#include "Component.h"


class PlayerInteractionComponent : public BaseComponent<PlayerInteractionComponent>
{
public:

	PlayerInteractionComponent::PlayerInteractionComponent(int id) : m_entityID(id)
	{
		m_coinTally = 30;
	}


	bool m_hasCannonBall;
	int m_entityID;
	int m_coinTally;

	PlayerInteractionComponent() 
	{
		m_hasCannonBall = false;
	}
	

	void pickUp(void * tuple,EventHandlerComponent* eventHanlderComponent)
	{
		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(tuple);
		int id = std::get<0>(m_tuple);
		

		if (id == m_entityID)
		{
			m_hasCannonBall = true;
			std::tuple<int, bool> * myTuple = new std::tuple<int, bool>{ m_entityID, m_hasCannonBall };
			Event * recieveCannonBall = new Event(Event::EventType::PickUpCannonBallAnimation, myTuple);
			EventHandlerSystem::AddEvent(recieveCannonBall, eventHanlderComponent);

		}
	}
	void drop(void * tuple,EventHandlerComponent* eventHanlderComponent)
	{
		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(tuple);
		int id = std::get<0>(m_tuple);
		
		if (id == m_entityID)
		{
			m_hasCannonBall = false;
			std::tuple<int,bool> * myTuple = new std::tuple<int,bool>{m_entityID, m_hasCannonBall };
			Event * recieveCannonBall = new Event(Event::EventType::DropCannonBallAnimation, myTuple);
			EventHandlerSystem::AddEvent(recieveCannonBall, eventHanlderComponent);
		}
	}


	void pickUpCoin(void * tuple)
	{
		std::tuple<int,int> m_tuple = *static_cast<std::tuple<int,int>*>(tuple);
		int id = std::get<0>(m_tuple);
		int amount = std::get<1>(m_tuple);
		if (id == m_entityID && m_coinTally < Constants::MAX_COIN_VALUE)
		{
			m_coinTally += amount;
		}
		if (id == -1)
		{
			m_coinTally += amount;
		}
	}
	void spendCoin(void * tuple)
	{
		std::tuple<int,int> m_tuple = *static_cast<std::tuple<int,int>*>(tuple);
		int id = std::get<0>(m_tuple);
		int price = std::get<1>(m_tuple);

		if (id == m_entityID)
		{
			if (m_coinTally >0)
			{
				m_coinTally -= price;
			}
		}
		if (id == -1)
		{
			if (m_coinTally - price >=0)
			{
				m_coinTally -= price;
			}
			else
			{
				m_coinTally = 0;
			}
		}
	}
};