#pragma once
#pragma once
#include "Component.h"
#include "NotificationComponent.h"

class NotificationHolderComponent : public BaseComponent<NotificationHolderComponent>
{
public:
	NotificationHolderComponent()
	{
		
	}

	void addButtonChangedEvent(void * buttonChangedTuple)
	{
		std::tuple<int,int> buttonTuple = *static_cast<std::tuple<int, int>*>(buttonChangedTuple);

		for (int i = 0; i < m_notificationComponents.size(); ++i)
		{
			m_notificationComponents.at(i)->changeTexture(buttonChangedTuple);
		}
	}

	std::vector<NotificationComponent *> m_notificationComponents;
};
