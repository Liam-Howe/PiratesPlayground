#pragma once
#include "StationComponent.h"
#include <SDL.h>
#include "AudioManager.h"

class EngineComponent : public StationComponent
{
public:
	EngineComponent(SDL_Point point, Entity * stationEntity,int cost, Vector2 glidePos) :
		StationComponent(point, stationEntity, cost, glidePos),
		m_sequenceCounter(0)
	{
		m_stationType = Engine;
	}

	void getNextButton(void * tuple)
	{
		int oldIndex = m_nextButtonToPress;
		int random = m_nextButtonToPress;

		while(random == m_nextButtonToPress)
			random = rand() % static_cast<int>(ControlComponent::Buttons::B + 1);

		m_nextButtonToPress = static_cast<ControlComponent::Buttons>(random);

		std::tuple<int, EventHandlerComponent *> buttonTuple = *static_cast<std::tuple<int, EventHandlerComponent *>*>(tuple);

		std::tuple<int, int> * myTuple = new std::tuple<int, int>{ m_nextButtonToPress, std::get<0>(buttonTuple),  };
		Event * buttonChange = new Event(Event::EventType::AddButtonChanged, myTuple);
		EventHandlerSystem::AddEvent(buttonChange, std::get<1>(buttonTuple));

		m_sequenceCounter++;

		if(m_sequenceCounter > Constants::_BUTTON_COUNT_PER_ENGINE_SEQUENCE)
		{
			AudioManager::Instance()->playSound(SoundEffectID::Success, 0);
			m_sequenceCounter = 0;
			BoatComponent::m_health = 1; // hacky
			BoatComponent::m_currBoatMaxSpeed = 0.4f;
			std::cout << "ship healed! " << BoatComponent::m_health << std::endl;
		}
	}

	void leaveEngine()
	{
		m_sequenceCounter = 0;
	}

	ControlComponent::Buttons m_nextButtonToPress;
	int m_sequenceCounter;

};
