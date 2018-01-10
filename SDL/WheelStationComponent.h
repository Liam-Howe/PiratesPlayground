#pragma once
#include "StationComponent.h"
#include "PositionComponent.h"
#include "BoatComponent.h"
#include "Application.h"


class WheelStationComponent : public StationComponent
{
public:
	WheelStationComponent(SDL_Point point, Entity * stationEntity, PositionComponent * boatPosComponent, VelocityComponent * boatVelComp, int cost, Vector2 glidePos) :
		StationComponent(point, stationEntity, cost, glidePos),
		m_angleIndex(std::vector<Vector2>(30)),
		m_boatPosComponent(boatPosComponent),
		m_boatVelComponent(boatVelComp)
	{
		m_canTakeInput = true;

		for (int i = 0; i < m_angleIndex.size(); ++i)
		{
			float degs = i * (360 / m_angleIndex.size());
			float radians = degs * (3.14159f / 180);

			m_angleIndex[i] = Vector2(cosf(radians), sinf(radians));
		}
		m_stationType = Wheel;
	}

	std::vector<Vector2> m_angleIndex;

	int m_currIndex;

	float m_time;


	const float m_boatTurningSpeed = 0.06f;

	const float m_boatMovingAccel = 0.015f;
	const float m_boatMovingVel = 0.7f;
	static const float m_boatMaxMovingVel;

	PositionComponent * m_boatPosComponent;
	VelocityComponent * m_boatVelComponent;
	static Entity * m_wheel;

	float m_timeSinceWheelNotifAnimUpdated;
	const float m_TIME_BETWEEN_ANIM_UPDATES = 0.1f;

	static bool m_canTakeInput;
	static void setWheel(Entity * wheel)
	{
		m_wheel = wheel;
	}
	void changeInputSettings()
	{
		m_canTakeInput = !m_canTakeInput;
	}

	void moveShip(void * tuple)
	{
		if (m_canTakeInput)
		{
			std::tuple<float, float, int , InputHandler*, Entity*> m_tuple = *static_cast<std::tuple<float, float, int ,InputHandler*, Entity* >*>(tuple);

			Vector2 dir = Vector2(std::get<0>(m_tuple), std::get<1>(m_tuple)).Normalize();
			Entity * wheel = (std::get<4>(m_tuple));

			if (dir == Vector2(0, 0))
				return;

			int closestIndex = -1;
			float closestDist = 999999;
			float currDist;

			for (int i = 0; i < m_angleIndex.size(); ++i)
			{
				currDist = Vector2::CalculateDistance(dir, m_angleIndex[i]);

				if (currDist < closestDist)
				{
					closestDist = currDist;
					closestIndex = i;
				}
			}

			int dirOverwrite = 0;

			int indexDist = m_currIndex - closestIndex;

			if (abs(indexDist) > m_angleIndex.size() / 2)
			{
				if (indexDist > 0)
				{
					dirOverwrite = 1;
				}
				else
				{
					dirOverwrite = -1;
				}
			}


			if (m_currIndex != closestIndex || dirOverwrite != 0)
			{
				if (closestIndex > m_currIndex && dirOverwrite != -1 && m_boatPosComponent->m_eulerRotation < 15)
				{
					moveWheelVelRight(wheel);
				}
				else if (closestIndex < m_currIndex && dirOverwrite != 1 && m_boatPosComponent->m_eulerRotation > -15)
				{
					moveWheelVelLeft(wheel);
				}
			}

			m_currIndex = closestIndex;
		}
	}

	void moveWheelRight( float speed = 0.003f, float turningVel = 0.2f)
	{
		//AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
		if (m_boatPosComponent->m_eulerRotation < 15)
			m_boatPosComponent->m_eulerRotation += turningVel;

		if (m_boatVelComponent->m_velocity.x  < m_boatMaxMovingVel)
			m_boatVelComponent->m_accel.x += speed;
		else
			m_boatVelComponent->m_velocity.x = m_boatMaxMovingVel;

		m_wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation+=3;
	}

	void moveWheelLeft(float speed = 0.003f, float turningVel = 0.2f)
	{
		//AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
		if (m_boatPosComponent->m_eulerRotation > -15)
			m_boatPosComponent->m_eulerRotation -= turningVel;

		if (m_boatVelComponent->m_velocity.x > -m_boatMaxMovingVel)
			m_boatVelComponent->m_accel.x -= speed;
		else
			m_boatVelComponent->m_velocity.x = -m_boatMaxMovingVel;


		m_wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation -= 3;
	}
	
	void moveWheelVelRight(Entity * wheel, float speed = 0.015f, float turningVel = 1.2f)
	{
		//AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
		if (m_boatPosComponent->m_eulerRotation < 15)
			m_boatPosComponent->m_eulerRotation += turningVel * BoatComponent::m_health;

		if (m_boatVelComponent->m_velocity.x < m_boatMovingVel)
			m_boatVelComponent->m_velocity.x += speed * BoatComponent::m_health;
		else
			m_boatVelComponent->m_velocity.x = m_boatMovingVel;
		wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation += 3 * BoatComponent::m_health;

	}

	void moveWheelVelLeft(Entity * wheel, float speed = 0.015f, float turningVel = 1.2f)
	{
		//AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
		if (m_boatPosComponent->m_eulerRotation > -15)
			m_boatPosComponent->m_eulerRotation -= turningVel * BoatComponent::m_health;

		if (m_boatVelComponent->m_velocity.x > -m_boatMovingVel)
			m_boatVelComponent->m_velocity.x -= speed * BoatComponent::m_health;
		else
			m_boatVelComponent->m_velocity.x = -m_boatMovingVel;

		wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation -= 3 * BoatComponent::m_health;
	}

	void decelerateShip(void* tuple)
	{
		std::tuple<Entity*> m_tuple = *static_cast<std::tuple<Entity* >*>(tuple);
		Entity * wheel = (std::get<0>(m_tuple));

		m_boatPosComponent->m_eulerRotation *= BoatComponent::m_RESET_ROTATION_SPEED;
		if (wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation > 0.1)
		{
			wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation--;
		}
		else if (wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation < 0.1)
		{
			wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation++;
		}
		else
		{
			wheel->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation = 0;
		}

		m_boatVelComponent->m_velocity.x *= BoatComponent::m_MOVEMENT_DEACCEL_RATE;
	}
};


