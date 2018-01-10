#pragma once
#include "Component.h"
#include "Vector2.h"

class CameraComponent : public BaseComponent<CameraComponent>
{
public:
	const SDL_Point SCREEN_SIZE {480, 540};
};
