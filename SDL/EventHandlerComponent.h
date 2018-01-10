#pragma once
#include<map>
#include<vector>
#include "EventListenerComponent.h"
#include "Event.h"

class EventHandlerComponent : public BaseComponent<EventHandlerComponent>
{
public:
	//dictionary holding a list of litener objs for each event type
	std::map<
		Event::EventType,
		std::vector<EventListenerComponent*>* //pointer to vec of eventlisteners
	> m_listeners;

	//ddictionary of messages
	std::vector<Event * > m_events;
};
