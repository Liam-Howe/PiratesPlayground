#pragma once
#include "Component.h"

class WorldRenderableComponent : public BaseComponent<WorldRenderableComponent>
{
public:
	SDL_Point m_screenPos;
};
