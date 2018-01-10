#pragma once
#include <unordered_map>
#include "Component.h"

class Entity {
public:

	Entity()
	{
		m_ID = ID;
		ID++;
	}

	~Entity()
	{
		if(m_ID > 0)
		{
			for (std::unordered_map<int, Component*>::iterator itr = m_map.begin(); itr != m_map.end(); itr++)
			{
				try
				{
					if (itr->first != 23)
						delete itr->second;
				}
				catch (...)
				{
					std::cout << "failed to delete, error: "<< std::endl;
				}
			}
		}
	}

	int m_ID;

	template <class T> T* getComponent(int type) 
	{
		return static_cast<T*>(m_map[type]);
	};
	template <class T> void addComponent(Component * component, int type) 
	{
		m_map[type] =  component;
	};

	static void resetGlobalID()
	{
		ID = 1;
	}

private:
	std::unordered_map<int, Component*> m_map;
	static int ID;
};

