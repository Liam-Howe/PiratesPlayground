#pragma once
#include "BaseSystem.h"
#include "AIControlSystem.h"


class BoatSystem : public BaseSystem
{
public:

	void update(float dt, EventHandlerComponent * eventHandlerComp)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			VelocityComponent * vel = m_entities.at(i)->getComponent<VelocityComponent>(ComponentTypes::Velocity);
			BoatComponent * boat = m_entities.at(i)->getComponent<BoatComponent>(ComponentTypes::Boat);

			if (boat->m_avgVels.size() < 10)
				boat->m_avgVels.push_back(vel->m_velocity.y);
			else
			{
				boat->m_avgVels.pop_back();
				boat->m_avgVels.insert(boat->m_avgVels.begin(), vel->m_velocity.y);

				float total = 0;
				bool isZero = false;
				for (int i = 0; i < boat->m_avgVels.size(); ++i)
				{
					if (boat->m_avgVels[i] == 0)
					{
						boat->m_averageVel = 0;
						isZero = true;
						break;
					}

					total += boat->m_avgVels[i];
				}

				if(isZero == false)
					boat->m_averageVel = total / boat->m_avgVels.size();


			}

			if (boat->m_canTakeDamage && boat->m_averageVel == 0)
			{
				boat->m_canTakeDamage = false;
				std::tuple<int, float> * tuple = new std::tuple<int, float>{ m_entities[0]->m_ID, 0.1f };
				Event * playerDamage = new Event(Event::EventType::Damage, tuple);
				EventHandlerSystem::AddEvent(playerDamage, eventHandlerComp);
				AudioManager::Instance()->playSound(SoundEffectID::Crash, 0);
			}
			else if(boat->m_averageVel != 0)
			{
				boat->m_canTakeDamage = true;
			}
				
			if (vel->m_velocity.x == 0)
				m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation *= 0.98f;
			else if (vel->m_velocity.x >= WheelStationComponent::m_boatMaxMovingVel)
				vel->m_velocity.x = WheelStationComponent::m_boatMaxMovingVel;
			else if(vel->m_velocity.x <= -WheelStationComponent::m_boatMaxMovingVel)
				vel->m_velocity.x = -WheelStationComponent::m_boatMaxMovingVel;

			float maxSpeed;

			boat->m_decayTime += dt;

			if ((boat->m_ENGINE_DECAY_TIME - boat->m_decayTime) / boat->m_ENGINE_DECAY_TIME < 0.3f)
				maxSpeed = -0.3 * boat->m_currBoatMaxSpeed;
			else
			{
				maxSpeed = -((boat->m_ENGINE_DECAY_TIME - boat->m_decayTime) / boat->m_ENGINE_DECAY_TIME) * boat->m_currBoatMaxSpeed;
			}
			
			if (vel->m_velocity.y > maxSpeed)
				vel->m_accel.y = -BoatComponent::m_MOVEMENT_ACCEL_RATE;
			else
			{
				vel->m_accel.y = 0;
			}
			

		}
	}

};
