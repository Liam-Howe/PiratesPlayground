#pragma once
#include "Vector2.h"
#include "SDL.h"

class Snapshot
{
public:
	Snapshot(Vector2 position, Vector2 vel, float rotation, int id)
		: m_pos(position)
		, m_vel(vel)
		, m_rot(rotation)
		, m_id(id)
	{
	}
	Vector2 m_pos;
	Vector2 m_vel;
	float m_rot;
	int m_id;
};

