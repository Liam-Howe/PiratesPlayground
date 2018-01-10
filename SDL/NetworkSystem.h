#include "NetworkComponent.h"
#include "BaseSystem.h"

class NetworkSystem : public BaseSystem
{
public:
	void update(float deltaTime);

	void addSnapshot(int id, Snapshot snapshot);

	float lerp(float a, float b, float f)
	{
		//std::cout << "ticks: " << SDL_GetTicks() << "time: " <<  f << std::endl;
		return a + f * (b - a);
	}
};

