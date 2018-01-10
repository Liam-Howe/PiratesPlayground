#pragma once
class Event
{
public:

	enum EventType
	{
		StopAnimation,
		StartWalkingAnimation,
		StartStationAnimation,
		SetSourceRect,
		RotateShip,
		FireCannon,
		ProduceCannonBall,
		DropCannonBall,
		PickUpCannonBall,
		PlayerRecieveCannonBall,
		PlayerDropCannonBall,
		MoveShip,
		DecelerateShip,
		DsiplayNotification,
		HideNotification,
		CoalShovelled,
		ChangeCoalInput,
		ChangeNotificationIcon,
		GetNextEngineButton,
		LeaveEngine,
		ResetButtonNeeded,
		AddButtonChanged,
		ChangeAccel,
		Loaded,
		FireCannonBall,
		StopStartWheelInput,
		IncrementRudderPress,
		Damage,
		LoadCannonBall,
		PickupCoin,
		SpendCoin,
		Respawn,
		DropCannonBallAnimation,
		PickUpCannonBallAnimation,
		SetNextWheelAnim,
		ResetWheelAnim

	};

	Event(EventType type, void * tuple) : m_type(type)
	{
		m_tuple = tuple;
	}

	~Event()
	{
		m_tuple = nullptr;
	}

	void * m_tuple;
	EventType m_type;
};