#pragma once
#include "Event.h"
#include <vector>
#include <iostream>

class Component
{
public:
	virtual int getType() const = 0;

	std::vector<Event *> m_events;
};

template <class T>
class BaseComponent : public Component
{
public:
	BaseComponent(int id = -1) : m_entityID(id)
	{
		
	}

	//the entity this component is attached to
	int m_entityID;
	static int type;
	int getType() const { return T::type; }
};
static int nextType = 0;
template <typename T> int BaseComponent<T>::type(nextType++);
