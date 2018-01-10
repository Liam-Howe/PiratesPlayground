#pragma once
#include "BaseSystem.h"


class HealthSystem : public BaseSystem
{
public:

	void update(EventHandlerComponent * eventHandlerComp)
	{
		for (int i = 0; i < m_entities.size(); i++)
		{
			BoatComponent * boatComponent = m_entities.at(i)->getComponent<BoatComponent>(ComponentTypes::Boat);

			boatComponent->m_currBoatMaxSpeed = boatComponent->m_health * boatComponent->m_BOAT_MAX_SPEED;

			if (boatComponent->m_health < 0.8)
			{
				if (boatComponent->m_protection == false)
				{
					int chance = rand() % 100 + 1;
					if (chance < 10)
					{
						if (WheelStationComponent::m_canTakeInput)
						{
							WheelStationComponent::m_canTakeInput = false;
							boatComponent->m_protection = true;
						}
					}
					else
					{
						boatComponent->m_protection = true;
					}
				}
			}
		}
	}
};