#include "stdafx.h"
#include "PositionComponent.h"
#include "NetworkComponent.h"
#include "NetworkSystem.h"
#include "VelocityComponent.h"
#include "Identifiers.h"

void NetworkSystem::update(float deltaTime)
{
	for (size_t i = 0; i < m_entities.size(); i++)
	{
		PositionComponent* pc = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);
		VelocityComponent* vc = m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity);
		NetworkComponent* nc = m_entities[i]->getComponent<NetworkComponent>(ComponentTypes::NetworkComponent);

		nc->m_elapsedTime += deltaTime;
		if (nc->m_snapshots.size() > 1)
		{
			if (nc->m_elapsedTime / 0.03 < 1)
			{
				pc->m_position.x = lerp(nc->m_snapshots[0].m_pos.x, nc->m_snapshots[1].m_pos.x, nc->m_elapsedTime / 0.03);
				pc->m_position.y = lerp(nc->m_snapshots[0].m_pos.y, nc->m_snapshots[1].m_pos.y, nc->m_elapsedTime / 0.03);
				/*vc->m_velocity.x = lerp(nc->m_snapshots[0].m_vel.x, nc->m_snapshots[1].m_vel.x, nc->m_elapsedTime / 0.1);
				vc->m_velocity.y = lerp(nc->m_snapshots[0].m_vel.y, nc->m_snapshots[1].m_vel.y, nc->m_elapsedTime / 0.1);*/
				pc->m_eulerRotation = lerp(nc->m_snapshots[0].m_rot, nc->m_snapshots[1].m_rot, nc->m_elapsedTime / 0.03);
			}
			else
			{
				vc->m_velocity = nc->m_snapshots[1].m_vel;
				nc->m_extrapolating = true;
			}
		}
	}
}

void NetworkSystem::addSnapshot(int id, Snapshot snapshot)
{
	NetworkComponent* nc = m_entities[id]->getComponent<NetworkComponent>(ComponentTypes::NetworkComponent); 
	nc->m_elapsedTime = 0;

	bool shouldAdd = true;
	if (nc->m_snapshots.size() > 2)
	{
		if (snapshot.m_id < nc->m_snapshots[1].m_id)
		{
			shouldAdd = false;
		}
	}


	if (shouldAdd)
	{
		nc->m_snapshots.push_back(snapshot);
		if (nc->m_snapshots.size() > 2)
		{
			nc->m_snapshots.erase(nc->m_snapshots.begin());
		}
		nc->m_snapshots[0].m_pos = m_entities[id]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position;
	}
	else
	{
		std::cout << "packets out of order" << snapshot.m_id << std::endl;
	}
}
