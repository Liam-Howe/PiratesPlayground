
#include "stdafx.h"
#include "BoatComponent.h"
const float BoatComponent::m_RESET_ROTATION_SPEED = 0.9f;
const float BoatComponent::m_MOVEMENT_DEACCEL_RATE = 0.05f;
const float BoatComponent::m_MOVEMENT_ACCEL_RATE = 0.005f;
const float BoatComponent::m_ENGINE_DECAY_TIME = 40;
const float BoatComponent::m_BOAT_MAX_SPEED = 0.2f;
float BoatComponent::m_currBoatMaxSpeed = 0.3f;

const float BoatComponent::m_COAL_REPLENISH_AMOUNT = 0.2f;

float BoatComponent::m_health;