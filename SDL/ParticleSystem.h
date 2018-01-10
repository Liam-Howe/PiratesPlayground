#pragma once
#include "BaseSystem.h"
#include "ParticleComponent.h"
#include "Identifiers.h"

class ParticleSystem : public BaseSystem
{
public:

	void update(float dt)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			ParticleComponent * particleComp = m_entities[i]->getComponent<ParticleComponent>(ComponentTypes::Particle);

			for (int j = 0; j < particleComp->m_particleManagers.size(); ++j)
			{
				ParticleManager * particleManager = &particleComp->m_particleManagers[j];

				particleManager->update(dt);
			}
		}
	}

	void render(SDL_Renderer * renderWindow)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			ParticleComponent * particleComp = m_entities[i]->getComponent<ParticleComponent>(ComponentTypes::Particle);

			for (int j = 0; j < particleComp->m_particleManagers.size(); ++j)
			{
				ParticleManager * particleManager = &particleComp->m_particleManagers[j];

				particleManager->render(renderWindow);
			}
		}
	}
};
