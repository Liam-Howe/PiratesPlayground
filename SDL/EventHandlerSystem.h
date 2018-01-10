#pragma once
#include "BaseSystem.h"
#include "EventHandlerComponent.h"

class EventHandlerSystem : BaseSystem
{
public:

	static void AddEvent(Event * event, EventHandlerComponent * handler)
	{
		handler->m_events.push_back(event);
	}

	void addListener(Event::EventType type, EventListenerComponent * listener, EventHandlerComponent * handler)
	{
		if (handler->m_listeners[type] == nullptr)
			handler->m_listeners.insert_or_assign(type, new std::vector<EventListenerComponent*>());

		handler->m_listeners[type]->push_back(listener);
	}

	void update(EventHandlerComponent * eventHandler)
	{
		for (int i = 0; i < eventHandler->m_events.size(); ++i)
		{
			Event * currEvent = eventHandler->m_events.at(i);

			if (eventHandler->m_listeners.find(currEvent->m_type) != eventHandler->m_listeners.end()) // if a listener for this event exists
			{
				std::vector<EventListenerComponent*> listenersForThisComp = *eventHandler->m_listeners[currEvent->m_type];

				for (int j = 0; j < listenersForThisComp.size(); ++j)
				{
					eventHandler->m_listeners[currEvent->m_type]->at(j)->m_events.push_back(currEvent);
				}
			}
		}

		eventHandler->m_events.clear();
	}
};
