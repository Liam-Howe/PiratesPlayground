#pragma once
#include "BaseSystem.h"
#include "Vector2.h"
#include "Identifiers.h"
#include "BoatAIComponent.h"


class BoatAISystem : public BaseSystem
{
public:

	std::vector<Vector2> * m_wayPointList;

	std::vector<Vector2> * getWaypoints()
	{
		if (m_wayPointList == nullptr)
		{
			m_wayPointList = new std::vector<Vector2>();
		}

		return m_wayPointList;
	}

	void update(float dt, Vector2 * boatPos)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			BoatAIComponent * boatComp = m_entities[i]->getComponent<BoatAIComponent>(ComponentTypes::BoatAI);
			
			if(boatComp->m_nextWaypoint == nullptr)
				findWaypoint(m_entities[i], boatPos);
			else if(boatPos->y < boatComp->m_nextWaypoint->y)
				findWaypoint(m_entities[i], boatPos);
		}
	}

	void findWaypoint(Entity * boatAI, Vector2 * boatPos)
	{
		BoatAIComponent * boatComp = boatAI->getComponent<BoatAIComponent>(ComponentTypes::BoatAI);

		float minDist = 99999;
		int minDistIndex = -1;
		float currDist = 0;

		for (int i = 0; i < m_wayPointList->size(); ++i)
		{
			if (m_wayPointList->at(i).y < boatPos->y)
			{
				currDist = Vector2::CalculateDistance(*boatPos, m_wayPointList->at(i));

				if (currDist < minDist)
				{
					minDist = currDist;
					minDistIndex = i;
				}
			}
		}
		
		if (minDistIndex != -1)
		{
			boatComp->m_nextWaypoint = &m_wayPointList->at(minDistIndex);
		}
		else
			boatComp->m_nextWaypoint = nullptr;
	}

};
