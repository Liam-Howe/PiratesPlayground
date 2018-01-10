#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Snapshot.h"

class NetworkComponent : public BaseComponent<NetworkComponent>
{
public:
	NetworkComponent() {};
	~NetworkComponent() {};
	bool m_extrapolating;
	float m_elapsedTime;
	std::vector<Snapshot> m_snapshots;
};
