#pragma once
#include "InputHandler.h"
#include "Entity.h"
#include "ControlComponent.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "StationsSystem.h"
#include "EventHandlerComponent.h"
#include "EventHandlerSystem.h"
#include <tuple>
#include "PlayerInteractionComponent.h"
#include "StationComponent.h"
#include "CannonStationComponent.h"
#include <cstdbool>
#include "ParticleComponent.h"
class ControlSystem : public BaseSystem
{
public:
	
	void handleInput(InputHandler * inputHandler, EventHandlerComponent * eventHandlerComp, StationsSystem * stationsSys, std::vector<Entity>& loadedCannonBalls, Entity& boat)
	{

		for (int i = 0; i < m_entities.size(); ++i)
		{
			ControlComponent * controlComp = m_entities.at(i)->getComponent<ControlComponent>(ComponentTypes::Control);
			PositionComponent * positionComponent = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position);
			VelocityComponent * velocityComponent = m_entities.at(i)->getComponent<VelocityComponent>(ComponentTypes::Velocity);
			InputHandler::Direction dir = inputHandler->getDirection(controlComp->m_ID);
			PlayerInteractionComponent * playerInteractionComponent = m_entities.at(i)->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction);
			SpriteComponent * spriteComponent = m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite);


			if (inputHandler->checkJoystickActive(controlComp->m_ID))
			{
				InputHandler::Direction dir = inputHandler->getDirection(controlComp->m_ID);

				if (controlComp->m_shouldTakeMovementInput)
				{
					velocityComponent->m_velocity = inputHandler->getLeftAnalogAxis(controlComp->m_ID) * 0.2f;

					if (dir != InputHandler::NoChange)
					{
						std::tuple<int, float, int,bool> * myTuple = new std::tuple<int, float, int,bool>{ m_entities[i]->m_ID, velocityComponent->m_velocity.Length() * 3.5f,  static_cast<AnimationType>(dir),m_entities[i]->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_hasCannonBall };
						Event * startAnimEvent = new Event(Event::EventType::StartWalkingAnimation, myTuple);
						EventHandlerSystem::AddEvent(startAnimEvent, eventHandlerComp);
					}
					else
					{
						std::tuple<int> * myTuple = new std::tuple<int>{ m_entities[i]->m_ID };
						Event * stopAnimEvent = new Event(Event::EventType::StopAnimation, myTuple);
						EventHandlerSystem::AddEvent(stopAnimEvent, eventHandlerComp);
					}
				}
			}

			SDL_Point gridPosition{ ((positionComponent->m_position.x + 32)) / 64, (positionComponent->m_position.y + 32) / 64 };

			int indexActive = stationsSys->checkDist(gridPosition);

			if (indexActive == -1)
			{
				if (inputHandler->isButtonPressed(controlComp->m_ID, JoyStickButtonID::Button_A))
				{
					if (playerInteractionComponent->m_hasCannonBall)
					{
						std::tuple<Vector2, int> * myTuple = new std::tuple<Vector2, int>{ positionComponent->m_position,controlComp->m_entityID };

						Event * dropCannonBall = new Event(Event::EventType::DropCannonBall, myTuple);
						EventHandlerSystem::AddEvent(dropCannonBall, eventHandlerComp);
						InputHandler::vibrate(controlComp->m_ID, 0.8, 250);
					}
					if (!playerInteractionComponent->m_hasCannonBall)
					{
						std::tuple<Vector2, int> * myTuple = new std::tuple<Vector2, int>{ positionComponent->m_position, controlComp->m_entityID };
						Event * pickUpCannonBall = new Event(Event::EventType::PickUpCannonBall, myTuple);
						EventHandlerSystem::AddEvent(pickUpCannonBall, eventHandlerComp);
						break;
					}
				}
				
				std::tuple<int> * playerPos = new std::tuple<int>{ m_entities.at(i)->m_ID };
				Event * hideNotification = new Event(Event::EventType::HideNotification, playerPos);
				EventHandlerSystem::AddEvent(hideNotification, eventHandlerComp);
			}

			bool shouldShowNotif = true;

			if (indexActive != -1)
			{
				if (stationsSys->checkShouldEnableInteraction(indexActive))
				{
					if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType == StationComponent::Cannon)
					{
						if (playerInteractionComponent->m_hasCannonBall && stationsSys->getEntity(indexActive)->getComponent<CannonStationComponent>(ComponentTypes::CannonStation)->m_loaded == false)
						{
							shouldShowNotif = true;
						}
						else if (playerInteractionComponent->m_hasCannonBall == false && stationsSys->getEntity(indexActive)->getComponent<CannonStationComponent>(ComponentTypes::CannonStation)->m_loaded)
						{
							shouldShowNotif = true;
						}
						else
						{
							shouldShowNotif = false;
						}
					}
					else if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType == StationComponent::CannonBall)
					{
						if (playerInteractionComponent->m_hasCannonBall)
						{
							shouldShowNotif = false;
						}
					}
					else if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType != StationComponent::CannonBall ||
						stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType != StationComponent::Cannon)
					{
						if (playerInteractionComponent->m_hasCannonBall)
						{
							shouldShowNotif = false;
						}
					}
					if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_EntityUsingID == -1 && shouldShowNotif)
					{
						if (m_entities[i]->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_coinTally >= stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_cost)
						{
							std::tuple<Vector2, int> * playerPos = new std::tuple<Vector2, int>{ positionComponent->m_position, m_entities.at(i)->m_ID };
							Event * dsiplayNotification = new Event(Event::EventType::DsiplayNotification, playerPos);
							EventHandlerSystem::AddEvent(dsiplayNotification, eventHandlerComp);
							if (inputHandler->isButtonPressed(controlComp->m_ID, JoyStickButtonID::Button_A))
							{
								if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType != StationComponent::Cannon)
								{
									AnimationType animationType;
									float animationSpeed;

									switch (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType)
									{
									case StationComponent::Type::CannonBall:
									{
										animationType = Up;
										animationSpeed = 0.5f;
										std::tuple<int, int> * spendTuple = new std::tuple<int, int>{ m_entities[i]->m_ID,stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_cost };
										Event * spendCoin = new Event(Event::EventType::SpendCoin, spendTuple);
										EventHandlerSystem::AddEvent(spendCoin, eventHandlerComp);
										AudioManager::Instance()->playSound(SoundEffectID::SpendCoin, 0);
										break;
									}
									case StationComponent::Type::Wheel:
									{
										animationType = Steering;
										animationSpeed = 0.5f;
										stationsSys->getEntity(indexActive)->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->m_time = SDL_GetTicks();
										std::tuple<int, int> * spendTuple = new std::tuple<int, int>{ m_entities[i]->m_ID,stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_cost };
										Event * spendCoin = new Event(Event::EventType::SpendCoin, spendTuple);
										EventHandlerSystem::AddEvent(spendCoin, eventHandlerComp);
										AudioManager::Instance()->playSound(SoundEffectID::SpendCoin, 0);
										break;
									}
									case StationComponent::Type::Engine:
										animationSpeed = 0.5f;
										animationType = Hammering;
										break;
									case StationComponent::Type::Furnace:
									{
										animationType = Shoveling;
										animationSpeed = 0.5f;
										stationsSys->getEntity(indexActive)->getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation)->m_time = SDL_GetTicks();
										std::tuple<int, int> * spendFurncae = new std::tuple<int, int>{ m_entities[i]->m_ID,stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_cost };
										Event * spendFurnaceCoin = new Event(Event::EventType::SpendCoin, spendFurncae);
										EventHandlerSystem::AddEvent(spendFurnaceCoin, eventHandlerComp);
										AudioManager::Instance()->playSound(SoundEffectID::SpendCoin, 0);
										break;
									}
									case StationComponent::Type::Rudder:
										animationType = Hammering;
										animationSpeed = 0.5f;
										break;
									default:
										animationType = Idling;
										animationSpeed = 0;
										break;
									}

									std::tuple<int, AnimationType, float> * m_Tuple = new std::tuple<int, AnimationType, float>{ m_entities[i]->m_ID, animationType, animationSpeed };
									controlComp->m_shouldTakeMovementInput = false;
									stationsSys->setActive(indexActive, eventHandlerComp, m_entities[i]->m_ID, controlComp->m_entityID);
									Event * startStationAnimation = new Event(Event::EventType::StartStationAnimation, m_Tuple);
									EventHandlerSystem::AddEvent(startStationAnimation, eventHandlerComp);
									velocityComponent->m_velocity = Vector2(0, 0);
									velocityComponent->m_accel = Vector2(0, 0);
									if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType == StationComponent::Wheel)
									{
										std::tuple<Vector2, int> * playerPos = new std::tuple<Vector2, int>{ positionComponent->m_position, m_entities.at(i)->m_ID };
										Event * hideNotification = new Event(Event::EventType::HideNotification, playerPos);
										EventHandlerSystem::AddEvent(hideNotification, eventHandlerComp);
										stationsSys->getEntity(indexActive)->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->m_time = SDL_GetTicks();
									}
								}
								else if (playerInteractionComponent->m_hasCannonBall && stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType == StationComponent::Cannon && !stationsSys->getEntity(indexActive)->getComponent<CannonStationComponent>(ComponentTypes::CannonStation)->m_loaded)
								{
									std::tuple<int> * myTuple = new std::tuple<int>{ m_entities[i]->m_ID };
									controlComp->m_shouldTakeMovementInput = false;
									stationsSys->setActive(indexActive, eventHandlerComp, m_entities[i]->m_ID, controlComp->m_entityID);
									
									velocityComponent->m_velocity = Vector2(0, 0);
									velocityComponent->m_accel = Vector2(0, 0);
									std::tuple<int> * tuple = new std::tuple<int>{ stationsSys->getEntity(indexActive)->m_ID };
									Event * loaded = new Event(Event::EventType::Loaded, tuple);
									EventHandlerSystem::AddEvent(loaded, eventHandlerComp);
									std::cout << "cannon ball has loaded " << std::endl;
									AudioManager::Instance()->playSound(SoundEffectID::loadSound, 0);

									std::tuple<int, EventHandlerComponent *> * m_tuple = new std::tuple<int, EventHandlerComponent*>{ m_entities[i]->m_ID,eventHandlerComp };
									Event * loadedCannonBall = new Event(Event::EventType::LoadCannonBall, m_tuple);
									EventHandlerSystem::AddEvent(loadedCannonBall, eventHandlerComp);
									if (stationsSys->getEntity(indexActive)->m_ID == 11)
									{
										loadedCannonBalls[0].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = true;
											
									}
									else
									{
										loadedCannonBalls[1].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = true;
									}

								}
								else if (stationsSys->getEntity(indexActive)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType == StationComponent::Cannon)
								{
									if (stationsSys->getEntity(indexActive)->getComponent<CannonStationComponent>(ComponentTypes::CannonStation)->m_loaded && !playerInteractionComponent->m_hasCannonBall)
									{
										std::tuple<int> * tuple = new std::tuple<int>{ stationsSys->getEntity(indexActive)->m_ID };
										Event * fireCannon = new Event(Event::EventType::FireCannon, tuple);
										EventHandlerSystem::AddEvent(fireCannon, eventHandlerComp);
										stationsSys->getEntity(indexActive)->getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TimedTurnOn(0.5);

										std::tuple<int> * playerPos = new std::tuple<int>{m_entities.at(i)->m_ID };
										Event * dsiplayNotification = new Event(Event::EventType::HideNotification, playerPos);
										EventHandlerSystem::AddEvent(dsiplayNotification, eventHandlerComp);


										InputHandler::vibrate(controlComp->m_ID, 0.8, 250);
										AudioManager::Instance()->playSound(SoundEffectID::FireCannon, 0);
										std::cout << "cannon ball has fired " << std::endl;
										boat.getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1].TimedTurnOn(0.5);
										if (stationsSys->getEntity(indexActive)->m_ID == 11)
										{
											loadedCannonBalls[0].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;

										}
										else
										{
											loadedCannonBalls[1].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;
										}
									}
								}
								
							}
							
						}
					}

				}
			}
		}
	}

};