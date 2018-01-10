#pragma once
#include "BaseSystem.h"
#include "AIControlComponent.h"
#include "Identifiers.h"
#include "EventHandlerSystem.h"
#include "StationsSystem.h"
#include "Node.h"
#include "PathFinder.h"
#include "BoatAIComponent.h"


class AIControlSystem : public BaseSystem
{
public:

	vector<StationComponent *> m_stations;
	static WheelStationComponent * wheel;


	void update(float dt, StationsSystem * stationsSys, BoatComponent * boatComp, EventHandlerComponent * eventHandlerComp, vector<vector<Entity>> * tileMap)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			AIControlComponent * aiControl = m_entities[i]->getComponent<AIControlComponent>(ComponentTypes::AIControl);
			PositionComponent * aiPos = m_entities[i]->getComponent<PositionComponent>(ComponentTypes::Position);

			checkIfShouldTakeNewStation(aiControl, aiPos, boatComp, m_entities[i], stationsSys, eventHandlerComp, tileMap);

			if (aiControl->m_stationOccupying != nullptr)
			{
				AnimationType animationType;
				float animationSpeed;

				switch (stationsSys->getEntity(stationsSys->checkDist(aiControl->m_stationOccupying->m_gridCoord))->getComponent<StationComponent>(ComponentTypes::Station)->m_stationType)
				{
				case StationComponent::Type::Wheel:
					animationType = Steering;
					animationSpeed = 0.5f;
					break;
				case StationComponent::Type::Engine:
					animationSpeed = 0.5f;
					animationType = Hammering;
					break;
				case StationComponent::Type::Furnace:
					animationType = Shoveling;
					animationSpeed = 0.5f;
					break;
				case StationComponent::Type::Rudder:
					animationType = Hammering;
					animationSpeed = 0.5f;
					break;
				default:
					animationType = Idling;
					animationSpeed = 0;
					break;
				}

				std::tuple<int, AnimationType, float> * myTuple = new std::tuple<int, AnimationType, float>{ m_entities[i]->m_ID, animationType, animationSpeed };
				Event * startStationAnimation = new Event(Event::EventType::StartStationAnimation, myTuple);
				EventHandlerSystem::AddEvent(startStationAnimation, eventHandlerComp);


				switch (aiControl->m_stationOccupying->m_stationType)
				{
				case StationComponent::Wheel:
					manageWheel(dt, aiControl, aiPos, m_entities[i]);
					break;
				case StationComponent::Cannon:
					break;
				case StationComponent::CannonBall:
					break;
				case StationComponent::Engine:
					manageEngine(stationsSys, aiControl, aiPos, dt, eventHandlerComp);
					break;

				case StationComponent::Furnace:
					manageFurnace(dt, aiControl, boatComp, eventHandlerComp);
					break;
				case StationComponent::Rudder:
					manageRudder(dt, aiControl, eventHandlerComp, m_entities[i], stationsSys);
				default: break;
				}
			}
			else
			{
				Vector2 velocity = m_entities[i]->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity;


				InputHandler::Direction dir = findVelDir(velocity);

				tuple<int, float, int,bool> * myTuple = new tuple<int, float, int, bool>{ m_entities[i]->m_ID, velocity.Length() * 3.5f,  static_cast<AnimationType>(dir),false };
				Event * startAnimEvent = new Event(Event::EventType::StartWalkingAnimation, myTuple);
				EventHandlerSystem::AddEvent(startAnimEvent, eventHandlerComp);
			}

		}
	}

	InputHandler::Direction findVelDir(Vector2 direction)
	{
		if (abs(direction.y) > abs(direction.x))
		{
			if (direction.y > 0)
				return InputHandler::Down;
			else
				return InputHandler::Up;
		}
		else
		{
			if (direction.x > 0)
				return InputHandler::Right;
			else
				return InputHandler::Left;
		}
	}

	void checkIfShouldTakeNewStation(AIControlComponent * aiControl, PositionComponent * aiPos, BoatComponent * boatComp, Entity * entity, StationsSystem * stationsSys, EventHandlerComponent * eventHandlerComp, std::vector<std::vector<Entity>> * tileMap)
	{
		float maxScore = 0;
		float currScore;
		int maxScoreIndex = -1;
		vector<Vector2> pathToBestStation;

		for (int j = 0; j < m_stations.size(); ++j)
		{
			StationComponent * station = m_stations[j];
			if ((station->m_EntityUsingID == -1 || station->m_EntityUsingID == entity->m_ID) && aiControl->m_readyToLeaveStation)
			{
				currScore = findStationScore(station, boatComp, aiControl, aiPos, entity);

				if (currScore > maxScore && (station->m_isAIAssigned == false || (station->m_isAIAssigned && station == aiControl->m_stationToAimFor)))
				{
					if (station->m_isAIAssigned == false)
					{
						maxScore = currScore;
						maxScoreIndex = j;
					}
					else if (station == aiControl->m_stationToAimFor || station->m_EntityUsingID == entity->m_ID)
					{
						maxScore = currScore * 2.0f;
						maxScoreIndex = j;
					}

				}
			}
		}

		if (maxScore == 0 && (m_stations[5]->m_EntityUsingID == -1 || m_stations[5]->m_EntityUsingID == entity->m_ID))
		{
			maxScoreIndex = 5;
		}
		else if (maxScore == 0)
		{
			maxScoreIndex = 3;
		}

		if (aiControl->m_stationOccupying != nullptr)
			if (aiControl->m_stationOccupying == m_stations[maxScoreIndex])
				maxScoreIndex = -1;

		if (maxScoreIndex != -1 && aiControl->m_readyToLeaveStation)
		{
			StationComponent * station = m_stations[maxScoreIndex];
			{
				SDL_Point startPosition{ ((aiPos->m_position.x) / 64), (aiPos->m_position.y + 32) / 64 };
				SDL_Point reversedCoord = { station->m_gridCoord.x, station->m_gridCoord.y };

				if (aiControl->m_pathToStation.size() != 0)
				{
					if (reversedCoord.x != aiControl->m_pathToStation[0].x && reversedCoord.y != aiControl->m_pathToStation[0].y)
					{
						pathToBestStation = findPath(SDL_Point{ startPosition.x, startPosition.y }, reversedCoord, tileMap);
					}
					else
					{
						pathToBestStation = findPath(SDL_Point{ startPosition.x, startPosition.y }, reversedCoord, tileMap);
					}
				}
				else
				{
					pathToBestStation = findPath(SDL_Point{ startPosition.x, startPosition.y }, reversedCoord, tileMap);
				}


				bool isEqual = true;



				if (pathToBestStation.size() > 1)
				{
					if (aiControl->m_pathToStation.size() == 0)
					{
						isEqual = false;
					}
					else
					{
						isEqual = false;
						if (pathToBestStation.size() > aiControl->m_pathToStation.size())
						{
							for (int i = 0; i < aiControl->m_pathToStation.size(); ++i)
							{
								if (pathToBestStation[i] == aiControl->m_pathToStation[i])
								{
									isEqual = true;
									break;
								}

							}
						}
						else
						{
							for (int i = 0; i < pathToBestStation.size(); ++i)
							{
								if (pathToBestStation[i] == aiControl->m_pathToStation[i])
								{
									isEqual = true;
									break;
								}
							}
						}
					}
				}


				if (isEqual == false)
				{
					if (aiControl->m_stationToAimFor != nullptr)
						aiControl->m_stationToAimFor->m_isAIAssigned = false;


					aiControl->m_pathToStation = pathToBestStation;

					aiControl->m_stationToAimFor = station;
					aiControl->m_stationToAimFor->m_isAIAssigned = true;

					if (aiControl->m_stationOccupying != nullptr)
						leaveStation(aiControl->m_stationOccupying, aiControl);

				}
				else if (aiControl->m_pathToStation.size() == 0)
				{
					enterStation(station, stationsSys, aiControl, eventHandlerComp, entity);
				}

				else
				{
					float dist = (tileMap->at(static_cast<int>(aiControl->m_pathToStation[0].x)).at(static_cast<int>(aiControl->m_pathToStation[0].y)).getComponent<PositionComponent>(ComponentTypes::Position)->m_position - aiPos->m_position).Length();
					if (dist < 40) // if at this best station
						enterStation(station, stationsSys, aiControl, eventHandlerComp, entity);
				}
			}
		}

		if (aiControl->m_pathToStation.size() != 0)
		{
			Vector2 stationCoord = tileMap->at(static_cast<int>(aiControl->m_pathToStation[aiControl->m_pathToStation.size() - 1].x)).at(static_cast<int>(aiControl->m_pathToStation[aiControl->m_pathToStation.size() - 1].y)).getComponent<PositionComponent>(ComponentTypes::Position)->m_position;

			Vector2 moveDir = stationCoord - aiPos->m_position;
			if (moveDir.Length() < 5)
			{
				aiControl->m_pathToStation.pop_back();

			}

			if (aiControl->m_pathToStation.size() == 0)
				entity->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity = Vector2(0, 0);
			else
				entity->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity = moveDir.Normalize() * 0.2f;

		}
		else
		{
			Vector2 moveDir = stationsSys->getEntity(stationsSys->checkDist(aiControl->m_stationOccupying->m_gridCoord))->getComponent<PositionComponent>(ComponentTypes::Position)->m_position - aiPos->m_position;

			if (Vector2::CalculateDistance(Vector2(0, 0), moveDir) > 5)
				entity->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity = moveDir.Normalize() * 0.2f;
			else
				entity->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity = Vector2(0, 0);
		}
	}

	float findStationScore(StationComponent * station, BoatComponent * boatComp, AIControlComponent * aiControl, PositionComponent * aiPos, Entity * entity)
	{
		float score = 0;
		float angle;


		const float wheelScoreScalar = 0.1f;
		const float engineScoreScalar = 3;
		const float furnaceScoreScalar = 1;

		switch (station->m_stationType)
		{
		case StationComponent::Wheel:
			angle = abs(findDir(aiControl, aiPos, entity));

			if (angle > Constants::ANGLE_NEEDED_FOR_AI_TO_TURNWHEEL * 4)
				score = (angle - Constants::ANGLE_NEEDED_FOR_AI_TO_TURNWHEEL);

			score *= wheelScoreScalar;

			if (WheelStationComponent::m_canTakeInput == false)
				score = 0;

			//cout << "wheel score: " << score << endl;

			break;
		case StationComponent::Cannon:
			break;
		case StationComponent::CannonBall:
			break;
		case StationComponent::Engine:
			score = 1 - BoatComponent::m_health;
			score *= engineScoreScalar;

			//cout << "engine score: " << score << endl;

			break;
		case StationComponent::Furnace:
			score = (abs(BoatComponent::m_currBoatMaxSpeed) * 1.2f) - abs(boatComp->m_velComp->m_velocity.y);
			score *= furnaceScoreScalar;

			//cout << "furnii score: " << score << endl;
			break;
		case StationComponent::Rudder:
			if (WheelStationComponent::m_canTakeInput == false)
				score = 50;
			else
				score = 0;

			//cout << "rudder score: " << score << endl;
			break;
		}

		return score;
	}

	enum DirectionToTurn
	{
		Left,
		Right,
		NoDir
	};

	float findDir(AIControlComponent * aiControl, PositionComponent * aiPos, Entity * entity)
	{
		Vector2 * nextWP = entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_nextWaypoint;

		if (nextWP == nullptr) // should leave
		{
			if(aiControl->m_stationOccupying != nullptr)
				leaveStation(wheel, aiControl);
			return -1;
		}

		Vector2 direction = *nextWP - entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<PositionComponent>(ComponentTypes::Position)->m_position;

		Vector2 vel = entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity * 1000;

		if (vel.y == 0)
			vel.y = -1;

		Vector2 xAxis = Vector2(-vel.y, vel.x).Normalize();

		if (xAxis.Length() == 0)
		{
			return direction.x * direction.y;
		}

		return (xAxis.x * direction.x) + (xAxis.y * direction.y);
	}

	void manageWheel(float dt, AIControlComponent * aiControl, PositionComponent * aiPos, Entity * entity)
	{
		float turner = findDir(aiControl, aiPos, entity);

		if (turner == -1)
			return;

		if (WheelStationComponent::m_canTakeInput == false)
		{
			aiControl->m_readyToLeaveStation = true;

			return;
		}


		aiControl->m_readyToLeaveStation = false;
		aiControl->m_timeSinceInput += dt;

		float xDiff = entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_nextWaypoint->x - entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x;


		if (abs(xDiff) < 10)
		{
			entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity.x = 0;
			entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel.x = 0;
			entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation *= 0.98f;
			aiControl->m_readyToLeaveStation = true;
		}
		else if (xDiff > 15)
			wheel->moveWheelRight(0.005f, 0.15f);
		else if (xDiff < -15)
			wheel->moveWheelLeft(0.005f, 0.15f);
		else
		{
			entity->getComponent<BoatAIComponent>(ComponentTypes::BoatAI)->m_boat->getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation *= 0.98f;
		}

	}

	void manageEngine(StationsSystem * stationSys, AIControlComponent * aiControl, PositionComponent * aiPos, float dt, EventHandlerComponent * eventHandler)
	{
		EngineComponent * engineComponent = nullptr;
		engineComponent = aiControl->m_stationOccupying->m_stationEntity->getComponent<EngineComponent>(ComponentTypes::Engine);

		aiControl->m_timeSinceInput += dt;

		if (aiControl->m_timeSinceInput > aiControl->m_timeBeforeInput)
		{
			aiControl->m_timeSinceInput = 0;
			aiControl->m_timeBeforeInput = (rand() % static_cast<int>(Constants::RANDOM_VARIATION_BETWEEN_INPUT * 1000.0f) / 1000.0f) + Constants::TIME_BETWEEN_AI_INPUT;

			std::tuple<int, EventHandlerComponent *> * tuple = new std::tuple<int, EventHandlerComponent *>{ aiControl->m_entityID, eventHandler };
			Event * changeButtonPressed = new Event(Event::EventType::GetNextEngineButton, tuple);
			EventHandlerSystem::AddEvent(changeButtonPressed, eventHandler);

			if (BoatComponent::m_health == 1)
				aiControl->m_readyToLeaveStation = true;
		}
	}

	void manageFurnace(float dt, AIControlComponent * aiControl, BoatComponent * boatComp, EventHandlerComponent * eventHandler)
	{
		aiControl->m_timeSinceInput += dt;

		if (aiControl->m_timeSinceInput > aiControl->m_timeBeforeInput)
		{
			aiControl->m_timeSinceInput = 0;
			aiControl->m_timeBeforeInput = (rand() % static_cast<int>(Constants::RANDOM_VARIATION_BETWEEN_INPUT * 1000.0f) / 1000.0f) + Constants::TIME_BETWEEN_AI_INPUT;

			Event * changeInput;
			Event * shovelCoal;
			//Event * changeNotificationTex;

			std::tuple<ControlComponent::Buttons, int> * tuple;

			if (aiControl->m_buttonNeeded == ControlComponent::Buttons::X)
				tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::B, aiControl->m_entityID };
			else
				tuple = new std::tuple<ControlComponent::Buttons, int>{ ControlComponent::Buttons::X, aiControl->m_entityID };

			aiControl->m_buttonNeeded = !aiControl->m_buttonNeeded;

			changeInput = new Event(Event::EventType::ChangeCoalInput, nullptr);
			EventHandlerSystem::AddEvent(changeInput, eventHandler);


			shovelCoal = new Event(Event::EventType::CoalShovelled, nullptr);
			EventHandlerSystem::AddEvent(shovelCoal, eventHandler);

			aiControl->m_readyToLeaveStation = true;
		}
	}

	void manageRudder(float dt, AIControlComponent * aiControl, EventHandlerComponent * eventHandler, Entity * entity, StationsSystem * stationsSystem)
	{
		RudderStation * rudderStationComponent = stationsSystem->getEntity(stationsSystem->checkDist(aiControl->m_stationOccupying->m_gridCoord))->getComponent<RudderStation>(ComponentTypes::RudderStation);

		aiControl->m_readyToLeaveStation = false;

		if (rudderStationComponent->m_amountOfButtonPressed >= Constants::BUTTON_COUNT_TO_FIX_RUDDER)
		{
			aiControl->m_readyToLeaveStation = true;
			leaveStation(aiControl->m_stationOccupying, aiControl);
			rudderStationComponent->m_amountOfButtonPressed = 0;
			if (WheelStationComponent::m_canTakeInput == false)
			{
				Event * changeInput = new Event(Event::EventType::StopStartWheelInput, nullptr);
				EventHandlerSystem::AddEvent(changeInput, eventHandler);
			}

			return;
		}

		aiControl->m_timeSinceInput += dt;

		if (aiControl->m_timeSinceInput > aiControl->m_timeBeforeInput)
		{
			aiControl->m_timeSinceInput = 0;
			aiControl->m_timeBeforeInput = (rand() % static_cast<int>(Constants::RANDOM_VARIATION_BETWEEN_INPUT * 1000.0f) / 1000.0f) + Constants::TIME_BETWEEN_AI_INPUT;
			Event * changeInput = new Event(Event::EventType::IncrementRudderPress, nullptr);
			EventHandlerSystem::AddEvent(changeInput, eventHandler);
		}
	}

	void enterStation(StationComponent * station, StationsSystem * stationsSys, AIControlComponent * aiControl, EventHandlerComponent * eventHandlerComp, Entity * entity)
	{
		aiControl->m_readyToLeaveStation = false;
		station->m_EntityUsingID = entity->m_ID;
		cout << "entering ID: " << station->m_EntityUsingID << endl;
		aiControl->m_stationOccupying = station;
		stationsSys->setActive(stationsSys->checkDist(station->m_gridCoord), eventHandlerComp, entity->m_ID, aiControl->m_entityID);
		tuple<int> * myTuple = new tuple<int>{ entity->m_ID };
		Event * startStationAnimation = new Event(Event::EventType::StartStationAnimation, myTuple);
		EventHandlerSystem::AddEvent(startStationAnimation, eventHandlerComp);
	}
	void leaveStation(StationComponent * stationToLeave, AIControlComponent * aiControl)
	{
		aiControl->m_stationOccupying->m_isAIAssigned = false;
		aiControl->m_stationOccupying->m_isActive = false;
		aiControl->m_stationOccupying->m_EntityUsingID = -1;
		aiControl->m_readyToLeaveStation = true;
		stationToLeave->m_entityID = -1;
		aiControl->m_oldStation = stationToLeave;
		aiControl->m_stationOccupying = nullptr;
	}

	vector<Vector2> findPath(SDL_Point startGridPos, SDL_Point endGridPos, std::vector<std::vector<Entity>> * tileMap)
	{
		vector<Node> path = PathFinder::instance()->FindPathToIndex(startGridPos, endGridPos, tileMap);
		vector<Vector2> returnPath;
		while (path.size() != 0)
		{
			Vector2 point = Vector2(path[0]._posInGrid.y, path[0]._posInGrid.x);
			returnPath.push_back(point);
			path.erase(path.begin());
		}
		return returnPath;
	}
};
