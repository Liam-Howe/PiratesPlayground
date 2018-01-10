#include "stdafx.h"
#include "WheelStationComponent.h"
bool WheelStationComponent::m_canTakeInput;
float const WheelStationComponent::m_boatMaxMovingVel = 0.15f;
Entity * WheelStationComponent::m_wheel;