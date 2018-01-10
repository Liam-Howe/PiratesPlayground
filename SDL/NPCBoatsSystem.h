#include "stdafx.h"
#pragma once
#include "PositionComponent.h"
#include "BoatAIComponent.h"
#include "VelocityComponent.h"
#include "BaseSystem.h"
#include "Identifiers.h"

class NPCBoatsSystem : public BaseSystem
{
public:

	void update()
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Entity * entity = m_entities[i];


			Vector2 * nextWP = entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_nextWaypoint;

			if (nextWP == nullptr) // should leave
			{
				return;
			}

			Vector2 direction = *nextWP - entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<PositionComponent>(ComponentTypes::Position)->m_position;

			Vector2 vel = entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity * 1000;
			Vector2 xAxis = Vector2(-vel.y, vel.x);


			float dp = (xAxis.x * direction.x) + (xAxis.y * direction.y);

			if (dp > 1000)
			{
				Vector2 * accel = &entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel;
				accel->x = 0.001f;
			}
			else if (dp < -1000)
			{
				Vector2 * accel = &entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel;
				accel->x = -0.001f;
			}
			else
			{
				entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation *= 0.98f;
			}
		}


	}
};
