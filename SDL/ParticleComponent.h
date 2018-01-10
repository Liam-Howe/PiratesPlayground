#pragma once
#include "Component.h"
#include "ParticleManager.h"

class ParticleComponent : public BaseComponent<ParticleComponent>
{
public: 
	
	ParticleComponent(std::vector<Vector2 *> posVec,std::vector<ParticleManager::ParticleSettings> settingsVec, SDL_Renderer * renderer = nullptr)
	{
		for (int i = 0; i < posVec.size(); ++i)
		{
			ParticleManager::ParticleSettings settings = settingsVec[i];
			settings._positionToParentTo = posVec[i];

			m_particleManagers.push_back(ParticleManager(settings, renderer));
		}
	}	
	

	std::vector<ParticleManager> m_particleManagers;
};
