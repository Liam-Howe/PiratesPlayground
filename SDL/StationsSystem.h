#pragma once
#include <memory>
#include "BaseSystem.h"
#include "StationComponent.h"
#include "WheelStationComponent.h"
#include "AnimationComponent.h"
#include "EventHandlerSystem.h"
#include "Entity.h"
#include "CameraComponent.h"
#include "CameraSystem.h"
#include "FurnaceStationComponent.h"
#include "NotificationComponent.h"
#include "EngineComponent.h"
#include "RudderStation.h"
#include "ParticleComponent.h"
#include "PlayerInteractionComponent.h"
#include "CannonStationComponent.h"
class StationsSystem : public BaseSystem
{
public:

	int checkDist(SDL_Point playerPos)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			StationComponent * station = m_entities.at(i)->getComponent<StationComponent>(ComponentTypes::Station);
			SDL_Point stationCoord = station->m_gridCoord;
			
			int xDiff = abs(stationCoord.x - playerPos.x);
			int yDiff = abs(stationCoord.y - playerPos.y);

			if (xDiff + yDiff < 2)
			{
				return i;
			}
		}
		return -1;
	}

	Entity * getEntity(int index)
	{
		return m_entities[index];
	}

	bool checkShouldEnableInteraction(int index)
	{
		StationComponent * station = m_entities.at(index)->getComponent<StationComponent>(ComponentTypes::Station);
		switch (station->m_stationType)
		{
		case StationComponent::Wheel: break;
		case StationComponent::Cannon: break;
		case StationComponent::CannonBall: break;
		case StationComponent::Engine:

			if (BoatComponent::m_health == 1)
				return false; 

			break;
		case StationComponent::Furnace: break;
		case StationComponent::Rudder:

			if (WheelStationComponent::m_canTakeInput)
				return false;
			break;

		default: ;
		}

		if (station->m_stationType == StationComponent::Rudder)
		{
		}
		else if (station->m_EntityUsingID != -1)
			return false;

		return true;
	}

	void setActive(int stationIndex, EventHandlerComponent * eventHandlerComp, int entityID, int playerID = -1)
	{
		m_entities.at(stationIndex)->getComponent<StationComponent>(ComponentTypes::Station)->m_isActive = true;
		m_entities.at(stationIndex)->getComponent<StationComponent>(ComponentTypes::Station)->m_EntityUsingID = entityID;
		
		Event * changeNotificationTex;
		Event * resetButton;
		std::tuple<ControlComponent::Buttons, int> * tuple;

		switch (m_entities.at(stationIndex)->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType)
		{
		case StationComponent::Engine:
			sendEngineButtonEvent(eventHandlerComp, playerID);
			break;
		case StationComponent::Furnace:
			tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::X, entityID };
			changeNotificationTex = new Event(Event::EventType::ChangeNotificationIcon, tuple);
			EventHandlerSystem::AddEvent(changeNotificationTex, eventHandlerComp);
			resetButton = new Event(Event::EventType::ResetButtonNeeded, tuple);
			EventHandlerSystem::AddEvent(resetButton, eventHandlerComp);
			break;
		case StationComponent::Wheel:
			//GameScene::m_steeringWheel->
			break;
		}

		return;
	}

	void setStationInactive(ControlComponent * playerControl, StationComponent * station, AnimationComponent * playerAnimation, EventHandlerComponent * eventHandlerComp, int i)
	{
		playerControl->m_shouldTakeMovementInput = true;
		station->m_isActive = false;
		playerAnimation->m_shouldUpdateAnimation = true;
		station->m_EntityUsingID = -1;
		station->m_isAIAssigned = false;

		std::tuple<ControlComponent::Buttons, int> * tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::A, playerControl->m_entityID };
		Event * changeNotificationTex = new Event(Event::EventType::ChangeNotificationIcon, tuple);
		EventHandlerSystem::AddEvent(changeNotificationTex, eventHandlerComp);

		std::tuple<int> * hidetuple = new std::tuple<int>{ playerControl->m_entityID };
		Event * hideNotif = new Event(Event::EventType::HideNotification, hidetuple);
		EventHandlerSystem::AddEvent(hideNotif, eventHandlerComp);
	
		if(station->m_stationType != StationComponent::CannonBall)
		{
			std::tuple<int, float, int, bool> * myTuple = new std::tuple<int, float, int, bool>{ playerControl->m_entityID, 0,  static_cast<int>(AnimationType::Idling), false };
			Event * startAnimEvent = new Event(Event::EventType::StartWalkingAnimation, myTuple);
			EventHandlerSystem::AddEvent(startAnimEvent, eventHandlerComp);
		}


	}

	void update(float dt, InputHandler * inputHandler, ControlComponent * playerControl, AnimationComponent * playerAnimation, EventHandlerComponent * eventHandlerComp, Entity * camera, Vector2 * playerPos, std::vector<Vector2*> playerPosVec,PlayerInteractionComponent* playerInteractionComponent, Entity * steeringWheel)
	{
		SDL_Point gridPosition;
		for (int i = 0; i < m_entities.size(); ++i)
		{
			StationComponent * station = m_entities.at(i)->getComponent<StationComponent>(ComponentTypes::Station);
			ParticleComponent * particleComponent = m_entities.at(i)->getComponent<ParticleComponent>(ComponentTypes::Particle);
			if (station->m_isActive)
			{
				if (playerControl->m_entityID == station->m_EntityUsingID)
				{
					
					if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_A) && playerControl->m_entityID == m_entities.at(i)->getComponent<StationComponent>(ComponentTypes::Station)->m_EntityUsingID)
					{
						setStationInactive(playerControl, station, playerAnimation, eventHandlerComp,i);
						return;
					}

					if(Vector2::CalculateDistance(*playerPos, m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position)->m_position) > 5)
					{
						Vector2 diff = m_entities.at(i)->getComponent<PositionComponent>(ComponentTypes::Position)->m_position - *playerPos;

						*playerPos += diff * 0.1f;

						std::tuple<Vector2, int> * playerPosTuple = new std::tuple<Vector2, int>{ *playerPos, playerControl->m_entityID };
						Event * dsiplayNotification = new Event(Event::EventType::DsiplayNotification, playerPosTuple);
						EventHandlerSystem::AddEvent(dsiplayNotification, eventHandlerComp);
					}



					Event * shovelCoal;
					Event * changeInput;
					Event * changeNotificationTex;
					FurnaceStationComponent * furnaceStationComponent;
					RudderStation * rudderStationComponent;
					EngineComponent * engineComp;
					std::tuple<ControlComponent::Buttons, int> * tuple;
					WheelStationComponent * wheel;

					switch (station->m_stationType)
					{
					case StationComponent::Wheel:

						wheel = m_entities[i]->getComponent<WheelStationComponent>(ComponentTypes::WheelStation);
						wheel->m_timeSinceWheelNotifAnimUpdated += dt;

						if(wheel->m_timeSinceWheelNotifAnimUpdated > wheel->m_TIME_BETWEEN_ANIM_UPDATES)
						{
							wheel->m_timeSinceWheelNotifAnimUpdated = 0;

							std::tuple<int> * tuple = new std::tuple<int>{ playerControl->m_entityID };
							changeNotificationTex = new Event(Event::EventType::SetNextWheelAnim, tuple);
							EventHandlerSystem::AddEvent(changeNotificationTex, eventHandlerComp);
						}

						if (inputHandler->getLeftAnalogAxis(playerControl->m_ID).x > (DEADZONE / MAX_AXIS_VALUE) || inputHandler->getLeftAnalogAxis(playerControl->m_ID).x < -(DEADZONE / MAX_AXIS_VALUE))
						{
							std::tuple<float, float, int, InputHandler*, Entity*> * myTuple = new std::tuple<float, float, int, InputHandler*, Entity*>{ inputHandler->getLeftAnalogAxis(playerControl->m_ID).x, inputHandler->getLeftAnalogAxis(playerControl->m_ID).y, playerControl->m_ID, inputHandler, steeringWheel};
							Event * moveShip = new Event(Event::EventType::MoveShip, myTuple);
							EventHandlerSystem::AddEvent(moveShip, eventHandlerComp);
							InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
						}

						if (SDL_GetTicks() - m_entities[i]->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->m_time > 3000)
						{
							playerInteractionComponent->m_coinTally--;
							m_entities[i]->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->m_time = SDL_GetTicks();
							if (playerInteractionComponent->m_coinTally <= 0)
							{
								setStationInactive(playerControl, station, playerAnimation, eventHandlerComp, i);
								AudioManager::Instance()->playSound(SoundEffectID::Error, 0);
								Vector2 accel = ((*playerPos - m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position)).Normalize() * 0.4f;
								std::tuple<Vector2, int> * accelTuple = new std::tuple<Vector2, int>{ accel, playerControl->m_entityID };
								Event * changeAccel = new Event(Event::EventType::ChangeAccel, accelTuple);
								EventHandlerSystem::AddEvent(changeAccel, eventHandlerComp);
							}
						}


					

						break;

					case StationComponent::Furnace:
					{
						
							furnaceStationComponent = m_entities.at(i)->getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation);

							if (furnaceStationComponent->m_buttonNeeded == FurnaceStationComponent::X)
							{
								if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_X))
								{
									AudioManager::Instance()->playSound(SoundEffectID::Shovelling, 0);
									InputHandler::vibrate(playerControl->m_ID,0.8,250);
									std::tuple<ControlComponent::Buttons, int> * tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::B, playerControl->m_entityID };
									changeNotificationTex = new Event(Event::EventType::ChangeNotificationIcon, tuple);
									EventHandlerSystem::AddEvent(changeNotificationTex, eventHandlerComp);
									
									changeInput = new Event(Event::EventType::ChangeCoalInput, nullptr);
									EventHandlerSystem::AddEvent(changeInput, eventHandlerComp);

									shovelCoal = new Event(Event::EventType::CoalShovelled, nullptr);
									EventHandlerSystem::AddEvent(shovelCoal, eventHandlerComp);

									particleComponent->m_particleManagers[2].TimedTurnOn(0.5);

								}
							}
							else
							{
								if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_B))
								{
									InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
									std::tuple<ControlComponent::Buttons, int> * tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::X, playerControl->m_entityID };
									AudioManager::Instance()->playSound(SoundEffectID::Shovelling, 0);
									changeNotificationTex = new Event(Event::EventType::ChangeNotificationIcon, tuple);
									EventHandlerSystem::AddEvent(changeNotificationTex, eventHandlerComp);

									changeInput = new Event(Event::EventType::ChangeCoalInput, nullptr);
									EventHandlerSystem::AddEvent(changeInput, eventHandlerComp);


									shovelCoal = new Event(Event::EventType::CoalShovelled, nullptr);
									EventHandlerSystem::AddEvent(shovelCoal, eventHandlerComp);
									particleComponent->m_particleManagers[2].TimedTurnOn(0.5);
								}
							}
						
							if (SDL_GetTicks() - m_entities[i]->getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation)->m_time > 3000)
							{
								playerInteractionComponent->m_coinTally --;
								m_entities[i]->getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation)->m_time = SDL_GetTicks();
								if (playerInteractionComponent->m_coinTally <= 0)
								{
									setStationInactive(playerControl, station, playerAnimation, eventHandlerComp, i);
									AudioManager::Instance()->playSound(SoundEffectID::Error, 0);
									Vector2 accel = ((*playerPos - m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position)).Normalize() * 0.4f;
									std::tuple<Vector2, int> * accelTuple = new std::tuple<Vector2, int>{ accel, playerControl->m_entityID };
									Event * changeAccel = new Event(Event::EventType::ChangeAccel, accelTuple);
									EventHandlerSystem::AddEvent(changeAccel, eventHandlerComp);
								}
							}
						break;
					}
					case StationComponent::Cannon:
					{
						for (int i = 0; i < Constants::MAX_PLAYERS_PER_SHIP; i++)
						{
							gridPosition = { ((int)(playerPosVec[i]->x + 32)) / 64, (int)((playerPosVec[i]->y + 32) / 64) };


							int xDiff = abs(station->m_gridCoord.x - gridPosition.x);
							int yDiff = abs(station->m_gridCoord.y - gridPosition.y);

							if (xDiff + yDiff < 2)
							{

								std::tuple<Vector2, int> * tuple = new std::tuple<Vector2, int>{ Vector2(playerPosVec[i]->x,playerPosVec[i]->y), playerControl->m_entityID };
								Event * addCannonBall = new Event(Event::EventType::Respawn, tuple);
								EventHandlerSystem::AddEvent(addCannonBall, eventHandlerComp);
								setStationInactive(playerControl, station, playerAnimation, eventHandlerComp, i);
								
								InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
								break;

							}
						}
						break;
					}
					case StationComponent::CannonBall:
						for (int i = 0; i < Constants::MAX_PLAYERS_PER_SHIP; i++)
						{
							gridPosition = { ((int)(playerPosVec[i]->x + 32)) / 64, (int)((playerPosVec[i]->y + 32) / 64) };


							int xDiff = abs(station->m_gridCoord.x - gridPosition.x);
							int yDiff = abs(station->m_gridCoord.y - gridPosition.y);

							if (xDiff + yDiff < 2)
							{
								std::tuple<int> * tuple = new std::tuple<int>{ playerControl->m_entityID };
								Event * addCannonBall = new Event(Event::EventType::ProduceCannonBall, tuple);
								EventHandlerSystem::AddEvent(addCannonBall, eventHandlerComp);
								InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
								setStationInactive(playerControl, station, playerAnimation, eventHandlerComp,i);
								break;
							}
						}
						break;

					case StationComponent::Engine:
						engineComp = m_entities[i]->getComponent<EngineComponent>(ComponentTypes::Engine);

						switch (engineComp->m_nextButtonToPress)
						{
						case ControlComponent::Buttons::X:
							if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_X))
							{
								sendEngineButtonEvent(eventHandlerComp, playerControl->m_entityID);
								InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
								AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
							}
							else if (inputHandler->didOtherButtonGetPressed(playerControl->m_ID, JoyStickButtonID::Button_X))
							{
								sendEngineButtonFailEvent(eventHandlerComp, playerControl->m_entityID, playerControl, station, playerAnimation, *playerPos, i);

							}
							break;
						case ControlComponent::Buttons::Y:
							if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_Y))
							{
								sendEngineButtonEvent(eventHandlerComp, playerControl->m_entityID);
								InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
								AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
							}
							else if (inputHandler->didOtherButtonGetPressed(playerControl->m_ID, JoyStickButtonID::Button_Y))
								sendEngineButtonFailEvent(eventHandlerComp, playerControl->m_entityID, playerControl, station, playerAnimation, *playerPos, i);
							break;
						case ControlComponent::Buttons::B:
							if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_B))
							{
								sendEngineButtonEvent(eventHandlerComp, playerControl->m_entityID);
								InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
								AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
							}
							else if (inputHandler->didOtherButtonGetPressed(playerControl->m_ID, JoyStickButtonID::Button_B))
								sendEngineButtonFailEvent(eventHandlerComp, playerControl->m_entityID, playerControl, station, playerAnimation, *playerPos, i);
							break;
						default: break;
						}


						if (BoatComponent::m_health == 1)
						{
							setStationInactive(playerControl, station, playerAnimation, eventHandlerComp, i);

							Vector2 accel = ((*playerPos - m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position)).Normalize() * 0.4f;
							std::tuple<Vector2, int> * accelTuple = new std::tuple<Vector2, int>{ accel, playerControl->m_entityID };
							Event * changeAccel = new Event(Event::EventType::ChangeAccel, accelTuple);
							EventHandlerSystem::AddEvent(changeAccel, eventHandlerComp);
						}



						break;

					case StationComponent::Rudder:
						rudderStationComponent = m_entities[i]->getComponent<RudderStation>(ComponentTypes::RudderStation);

						tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::X, playerControl->m_entityID };

						changeNotificationTex = new Event(Event::EventType::ChangeNotificationIcon, tuple);
						EventHandlerSystem::AddEvent(changeNotificationTex, eventHandlerComp);

						if (rudderStationComponent->m_amountOfButtonPressed >= Constants::BUTTON_COUNT_TO_FIX_RUDDER)
						{
							setStationInactive(playerControl, station, playerAnimation, eventHandlerComp,i);
							rudderStationComponent->m_amountOfButtonPressed = 0;
							changeInput = new Event(Event::EventType::StopStartWheelInput, nullptr);
							EventHandlerSystem::AddEvent(changeInput, eventHandlerComp);
							AudioManager::Instance()->playSound(SoundEffectID::Success, 0);
							Vector2 accel = ((*playerPos - m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position)).Normalize() * 0.4f;
							std::tuple<Vector2, int> * accelTuple = new std::tuple<Vector2, int>{ accel, playerControl->m_entityID };
							Event * changeAccel = new Event(Event::EventType::ChangeAccel, accelTuple);
							EventHandlerSystem::AddEvent(changeAccel, eventHandlerComp);
							return;
						}

						if (inputHandler->isButtonPressed(playerControl->m_ID, JoyStickButtonID::Button_X))
						{
							changeInput = new Event(Event::EventType::IncrementRudderPress, nullptr);
							EventHandlerSystem::AddEvent(changeInput, eventHandlerComp);
							InputHandler::vibrate(playerControl->m_ID, 0.8, 250);
							AudioManager::Instance()->playSound(SoundEffectID::FixingRudder, 0);
						}

						break;
					}
				}
			}
			else
			{
				Event * decelerateShip;
				Event * resetAnim;
				switch (station->m_stationType)
				{
				case StationComponent::Wheel:	
					std::tuple<Entity*> * wheelTuple = new std::tuple<Entity*>{ steeringWheel };
					decelerateShip = new Event(Event::EventType::DecelerateShip, wheelTuple);
					EventHandlerSystem::AddEvent(decelerateShip, eventHandlerComp);

					resetAnim = new Event(Event::EventType::ResetWheelAnim, nullptr);
					EventHandlerSystem::AddEvent(resetAnim, eventHandlerComp);
					break;
				}
			}

		}
	}

	void sendEngineButtonEvent(EventHandlerComponent * eventHandler, int playerID)
	{
		std::tuple<int, EventHandlerComponent *> * tuple = new std::tuple<int, EventHandlerComponent *>{ playerID, eventHandler };
		Event * changeButtonPressed = new Event(Event::EventType::GetNextEngineButton, tuple);
		EventHandlerSystem::AddEvent(changeButtonPressed, eventHandler);

	}

	void sendEngineButtonFailEvent(EventHandlerComponent * eventHandler, int playerID, ControlComponent * playerControl, StationComponent * station, AnimationComponent * playerAnimation, Vector2 playerPos, int stationIndex)
	{
		AudioManager::Instance()->playSound(SoundEffectID::Error, 0);
		Event * changeButtonPressed = new Event(Event::EventType::LeaveEngine, nullptr);
		EventHandlerSystem::AddEvent(changeButtonPressed, eventHandler);

		setStationInactive(playerControl, station, playerAnimation, eventHandler,stationIndex);


		Vector2 accel = ((playerPos - m_entities[stationIndex]->getComponent<PositionComponent>(ComponentTypes::Position)->m_position)).Normalize() * 0.4f;
		std::tuple<Vector2, int> * accelTuple = new std::tuple<Vector2, int>{ accel, playerControl->m_entityID };
		Event * changeAccel = new Event(Event::EventType::ChangeAccel, accelTuple);
		EventHandlerSystem::AddEvent(changeAccel, eventHandler);
		
	}
};
