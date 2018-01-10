#pragma once
#include "Component.h"
#include "VelocityComponent.h"
#include <queue>
#include <SDL.h>

class BoatComponent : public BaseComponent<BoatComponent>
{
public:
	BoatComponent(VelocityComponent * boatVelocityComp, int entityID)
		:m_decayTime(0),
		 m_velComp(boatVelocityComp),
		BaseComponent<BoatComponent>(entityID),
		m_canTakeDamage(false)
	{
		m_health = 1;
		m_protection = true;
	}

	static const float m_RESET_ROTATION_SPEED;
	static const float m_MOVEMENT_DEACCEL_RATE;
	static const float m_MOVEMENT_ACCEL_RATE;
	static const float m_ENGINE_DECAY_TIME;
	static const float m_BOAT_MAX_SPEED;
	static float m_currBoatMaxSpeed;
	static const float m_COAL_REPLENISH_AMOUNT;

	std::vector<float> m_prevPosList;

	float m_decayTime;
	VelocityComponent * m_velComp;
	static float m_health;
	bool m_protection;

	bool m_canTakeDamage;
	std::vector < float > m_avgVels;
	float m_averageVel;

	void shovelCoal(void * tuple)
	{
		if(m_decayTime > 0)
		{
			if (m_decayTime > m_ENGINE_DECAY_TIME)
				m_decayTime = m_ENGINE_DECAY_TIME;

			m_decayTime -= (m_COAL_REPLENISH_AMOUNT * m_ENGINE_DECAY_TIME);
			m_velComp->m_velocity.y += -m_currBoatMaxSpeed * m_COAL_REPLENISH_AMOUNT;
		}
	}

	void takeDamage(void * m_Tuple)
	{
		std::tuple<int, float> m_tuple = *static_cast<std::tuple<int, float>*>(m_Tuple);
		int id = std::get<0>(m_tuple);
		float damage = std::get<1>(m_tuple);

		if (id == BoatComponent::m_entityID)
		{
			if (m_health > 0.1)
			{
				m_protection = false;
				m_health -= damage;
			}
			
		}
	}

};
