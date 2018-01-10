#pragma once
#include "Component.h"
#include "Vector2.h"
#include <tuple>

class VelocityComponent : public BaseComponent<VelocityComponent>
{
public:

	VelocityComponent(Vector2 vel, int entityID, float friction = 0.9f)
		: BaseComponent<VelocityComponent>(entityID),
		  m_velocity(vel),
		  m_angularAccel(0),
		  m_timeSinceUpdatedPos(0),
		  m_friction(friction)
	{
		
	}

	Vector2 m_velocity;
	float m_angularAccel;
	const float VELOCITY_SCALAR = 800;
	float m_friction;
	Vector2 m_accel;
	float m_timeSinceUpdatedPos;
	const float m_velTimeStep = 0.01f;

	void changeAccel(void * tuple)
	{
		std::tuple<Vector2, int> accelTuple = *static_cast<std::tuple<Vector2, int>*>(tuple);

		if(m_entityID == std::get<1>(accelTuple))
			m_accel = std::get<0>(accelTuple);
	}
};
