#pragma once
#include "Component.h"
#include "Vector2.h"

class SpatialComponent : public BaseComponent<SpatialComponent>
{
public:
	Vector2 m_position;
	Vector2 m_velocity;
	const float VELOCITY_SCALAR = 200;
};
