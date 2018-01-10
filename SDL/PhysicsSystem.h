#pragma once
#include "BaseSystem.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"

class PhysicsSystem : public BaseSystem
{
public:


	void update(float deltaTime)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			VelocityComponent * velocityComponent = m_entities.at(i)->getComponent<VelocityComponent>(ComponentTypes::Velocity);
			ParticleComponent * particleComponent = m_entities.at(i)->getComponent<ParticleComponent>(ComponentTypes::Particle);
			PositionComponent * positionComponent = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position);
			positionComponent->m_previousPosition = positionComponent->m_position;
			positionComponent->m_position += velocityComponent->m_velocity * deltaTime * velocityComponent->VELOCITY_SCALAR;

			velocityComponent->m_timeSinceUpdatedPos += deltaTime;


			while (velocityComponent->m_timeSinceUpdatedPos > velocityComponent->m_velTimeStep)
			{
				positionComponent->m_eulerRotation += velocityComponent->m_angularAccel;
				velocityComponent->m_timeSinceUpdatedPos -= velocityComponent->m_velTimeStep;

				velocityComponent->m_velocity += velocityComponent->m_accel;

				velocityComponent->m_accel *= velocityComponent->m_friction;
				positionComponent->m_position += velocityComponent->m_velocity * deltaTime * velocityComponent->VELOCITY_SCALAR;
			}
			velocityComponent->m_timeSinceUpdatedPos = 0;

			if (particleComponent != NULL)
			{
				if ((abs(velocityComponent->m_velocity.x) > 0) || (abs(velocityComponent->m_velocity.y) > 0))
				{
					particleComponent->m_particleManagers[0].TurnOn();
					
					//AudioManager::Instance()->playSound(SoundEffectID::Footsteps, 0);	
				}
				else
				{
					particleComponent->m_particleManagers[0].TurnOff();	
				}
			}
		}
	}

};
