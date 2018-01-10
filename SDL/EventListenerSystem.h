#pragma once
#include "BaseSystem.h"
#include "EventListenerComponent.h"
#include "Event.h"
#include "CannonBallComponent.h"
#include "DummyCannonBallComponent.h"
#include "PlayerInteractionComponent.h"
#include "BoatComponent.h"
#include "NotificationComponent.h"
#include "NotificationsHolder.h"
#include "CannonStationComponent.h"
#include "WheelStationComponent.h"
#include "FurnaceStationComponent.h"
#include "EngineComponent.h"
#include "RudderStation.h"

class EventListenerSystem: public BaseSystem
{
public: 
	void update(EventHandlerComponent * eventHandler)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			EventListenerComponent * listener = m_entities.at(i)->getComponent<EventListenerComponent>(ComponentTypes::EventListener);

			for (int j = 0; j < listener->m_events.size(); ++j)
			{
				Event * event = listener->m_events.at(j);

				switch (event->m_type)
				{
				case Event::StopAnimation:
					m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation)->stopAnimation(event->m_tuple);
					break;
				case Event::StartWalkingAnimation:
					m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation)->startWalkingAnimation(event->m_tuple);
					break;
				case Event::StartStationAnimation:
					m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation)->startStationAnim(event->m_tuple);
					break;
				case Event::SetSourceRect:
					m_entities.at(i)->getComponent<SpriteComponent>(ComponentTypes::Sprite)->setSourceRect(event->m_tuple);
					break;
				case Event::MoveShip:
					m_entities.at(i)->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->moveShip(event->m_tuple);
					break;
				case Event::DecelerateShip:
					m_entities.at(i)->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->decelerateShip(event->m_tuple);
					break;
				case Event::DsiplayNotification: 
					m_entities.at(i)->getComponent<NotificationComponent>(ComponentTypes::Notification)->setState(event->m_tuple);
					break;
				case Event::HideNotification:
					m_entities.at(i)->getComponent<NotificationComponent>(ComponentTypes::Notification)->hide(event->m_tuple);
					break;
				case Event::ChangeNotificationIcon:
					m_entities.at(i)->getComponent<NotificationComponent>(ComponentTypes::Notification)->changeTexture(event->m_tuple);
					break;
				case Event::CoalShovelled:
					m_entities.at(i)->getComponent<BoatComponent>(ComponentTypes::Boat)->shovelCoal(event->m_tuple);
					break;
				case Event::ChangeCoalInput:
					m_entities.at(i)->getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation)->changeButtonNeeded();
					break;
				case Event::ProduceCannonBall:
					m_entities.at(i)->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall)->spawn(eventHandler, event->m_tuple);
					break;
				case Event::DropCannonBall:
					m_entities.at(i)->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall)->drop(event->m_tuple, eventHandler);
					break;
				case Event::PickUpCannonBall:
					m_entities.at(i)->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall)->pickUp(event->m_tuple, eventHandler);
					break;
				case Event::PlayerRecieveCannonBall:
					m_entities.at(i)->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->pickUp(event->m_tuple,eventHandler);
					
					break;
				case Event::PlayerDropCannonBall:
					m_entities.at(i)->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->drop(event->m_tuple,eventHandler);
					break;
				case Event::DropCannonBallAnimation:
					m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation)->hasCannonBall(event->m_tuple,eventHandler);
					break;
				case Event::PickUpCannonBallAnimation:
					m_entities.at(i)->getComponent<AnimationComponent>(ComponentTypes::Animation)->hasCannonBall(event->m_tuple,eventHandler);
					break;
				case Event::ResetButtonNeeded:
					m_entities.at(i)->getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation)->resetButtonNeeded();
					break;
				case Event::GetNextEngineButton:
					m_entities.at(i)->getComponent<EngineComponent>(ComponentTypes::Engine)->getNextButton(event->m_tuple);
					break;
				case Event::AddButtonChanged:
					m_entities.at(i)->getComponent<NotificationHolderComponent>(ComponentTypes::NotificationHolder)->addButtonChangedEvent(event->m_tuple);
					break;
				case Event::LeaveEngine:
					m_entities.at(i)->getComponent<EngineComponent>(ComponentTypes::Engine)->leaveEngine();
					break;
				case Event::ChangeAccel:
					m_entities.at(i)->getComponent<VelocityComponent>(ComponentTypes::Velocity)->changeAccel(event->m_tuple);
					break;
				case Event::StopStartWheelInput:
					m_entities.at(i)->getComponent<WheelStationComponent>(ComponentTypes::WheelStation)->changeInputSettings();
					break;
				case Event::IncrementRudderPress:
					m_entities.at(i)->getComponent<RudderStation>(ComponentTypes::RudderStation)->incrementButtonPress();
					break;
				case Event::Loaded:
					m_entities.at(i)->getComponent<CannonStationComponent>(ComponentTypes::CannonStation)->loadCannon(event->m_tuple);
					break;
				case Event::LoadCannonBall:
					m_entities.at(i)->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall)->loadCannon(event->m_tuple);
					break;
				case Event::FireCannon:
					m_entities.at(i)->getComponent<CannonStationComponent>(ComponentTypes::CannonStation)->fireCannon(eventHandler, event->m_tuple);
					break;
				case Event::FireCannonBall:
					m_entities.at(i)->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall)->fireCannonBall();
					break;
				case Event::Damage:
					m_entities.at(i)->getComponent<BoatComponent>(ComponentTypes::Boat)->takeDamage(event->m_tuple);
					break;
				case Event::SpendCoin:
					m_entities.at(i)->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->spendCoin(event->m_tuple);
					break;
				case Event::PickupCoin:
					m_entities.at(i)->getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->pickUpCoin(event->m_tuple);
					break;
				case Event::SetNextWheelAnim:
					m_entities.at(i)->getComponent<NotificationComponent>(ComponentTypes::Notification)->setNextNotificationAnim(event->m_tuple);
					break;
				case Event::ResetWheelAnim:
					m_entities.at(i)->getComponent<NotificationComponent>(ComponentTypes::Notification)->m_frameCounter = 0;
					break;
				default:
					break;
				}

			}
			listener->m_events.clear();
		}

	}
};
