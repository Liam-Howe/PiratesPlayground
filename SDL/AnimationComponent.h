#pragma once
#include "Component.h"
#include  <tuple>
#include "EventHandlerSystem.h"


enum AnimationType
{
	Up,
	Down,
	Left, 
	Right,
	Steering,
	Shoveling,
	Hammering,
	Cannon,
	Idling,

};

class AnimationComponent : public BaseComponent<AnimationComponent>
{

	struct StartMessageData
	{
		float speedScalar;
		InputHandler::Direction direction;
	};


public:
	AnimationComponent(int entityID):
		BaseComponent(entityID),
		m_currentFrame(0),
		m_frameSize{64,64},
		m_frameTimer(0),
		m_speedScalar(0),
		m_timeBetweenFrames(0.07f),
		m_numOfFrames(3), 
		m_animationType(AnimationType::Down),
		m_shouldUpdateAnimation(true),
		offset(0)
	{
	}

	AnimationComponent(int entityID, SDL_Point frameSize, float timeBetweenFrames, int numOfFrames) :
		BaseComponent(entityID),
		m_currentFrame(0),
		m_frameSize(frameSize),
		m_frameTimer(0),
		m_speedScalar(1),
		m_timeBetweenFrames(timeBetweenFrames),
		m_numOfFrames(numOfFrames),
		m_animationType(AnimationType::Up),
		m_shouldUpdateAnimation(true),
		offset(0)
	{
	}

	int m_currentFrame;
	SDL_Point m_frameSize;
	float m_frameTimer;
	float m_speedScalar;
	float m_timeBetweenFrames;
	int m_numOfFrames;
	AnimationType m_animationType;

	bool m_shouldUpdateAnimation;
	int offset;

	//event handler funcs
	void stopAnimation(void * tuple)
	{
		std::tuple<int> m_tuple = *static_cast<std::tuple<int>*>(tuple);
		if (m_entityID == std::get<0>(m_tuple))
		{
			m_speedScalar = 0;
			m_frameTimer = 0;
			m_currentFrame = 0;
		}
	}

	void startWalkingAnimation(void * tuple)
	{
		std::tuple<int, float, AnimationType,bool> m_tuple = *static_cast<std::tuple<int, float, AnimationType,bool>*>(tuple);
		if (m_entityID == std::get<0>(m_tuple) && std::get<3>(m_tuple) == true)
		{
			m_speedScalar = std::get<1>(m_tuple);
			m_animationType = static_cast<AnimationType>(std::get<2>(m_tuple) + 8);
			m_shouldUpdateAnimation = true;
		}
		else if (m_entityID == std::get<0>(m_tuple) && std::get<3>(m_tuple) == false)
		{
			m_speedScalar = std::get<1>(m_tuple);
			m_animationType = static_cast<AnimationType>(std::get<2>(m_tuple));
			m_shouldUpdateAnimation = true;
		}
	}
	void hasCannonBall(void * tuple, EventHandlerComponent* evenHandler)
	{
		std::tuple<int,bool> m_tuple = *static_cast<std::tuple<int,bool>* > (tuple);
		if (std::get<1>(m_tuple) == true && std::get<0>(m_tuple) == m_entityID)
		{

			offset = 8;
			if (m_animationType < static_cast<AnimationType>(offset))
			{
				m_animationType = static_cast<AnimationType>(m_animationType + offset);
			}
			std::tuple<int, int, int> * sourceTuple = new std::tuple<int, int, int>{ int(0) , int(m_animationType * m_frameSize.y), m_entityID };
			Event * setSourceRect = new Event(Event::EventType::SetSourceRect, sourceTuple);
			m_currentFrame = 0;
			EventHandlerSystem::AddEvent(setSourceRect, evenHandler);
			std::cout << m_animationType << std::endl;
		}
		else if (std::get<1>(m_tuple) == false && std::get<0>(m_tuple) == m_entityID)

		{
			offset = 8;
			if (m_animationType > static_cast<AnimationType>(offset))
			{
				m_animationType = static_cast<AnimationType>(m_animationType - offset);
			}
			
			std::tuple<int, int, int> * myTuple = new std::tuple<int, int, int>{ int(0) , int(m_animationType * m_frameSize.y), m_entityID };
			Event * setSourceRect = new Event(Event::EventType::SetSourceRect, myTuple);
			m_currentFrame = 0;
			EventHandlerSystem::AddEvent(setSourceRect, evenHandler);
			std::cout << m_animationType << std::endl;
			
		}
		
	}

   	void startStationAnim(void * tuple)
	{
		std::tuple<int, AnimationType, float> m_tuple = *static_cast<std::tuple<int, AnimationType, float>*>(tuple);
		if (m_entityID == std::get<0>(m_tuple))
		{
			m_animationType = std::get<1>(m_tuple);
			m_speedScalar = std::get<2>(m_tuple);
			m_shouldUpdateAnimation = true;
		}
	}

};

