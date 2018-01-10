#pragma once
namespace Actions
{
	enum ID
	{
		Push,
		Pop,
		Clear,
	};
}
namespace JoyStickButtonID
{
	enum ID
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_L,
		Button_R,
		Button_Back,
		Button_Start,
		L_Thumbstick_Click,
		R_Thumbstick_Click,
	};
}

namespace ComponentTypes
{
	enum Type
	{
		AIControl,
		Animation,
		Boat,
		BoatAI,
		Camera,
		CannonBall,
		CannonBallStation,
		CannonStation,
		Collision,
		Component,
		Control,
		DummyCannonBall,
		Engine,
		EventHandler,
		EventListener,
		FurnaceStation,
		Notification,
		NotificationHolder,
		Particle,
		PlayerInteraction,
		Position,
		RudderStation,
		Sprite,
		Station,
		Tile,
		Velocity,
		WheelStation,
		WorldRenderable,
		AliveComponent,
		RemotePlayerComponent,
		CoinComponent,
		NetworkComponent,
		CoinBarComponent,
		LoadedCannonComponent,
	};
}

namespace JoyStickID
{
	enum ID
	{
		One,
		Two,
		Three,
		Four
	};
}
namespace AnalogStickID
{
	enum ID
	{
		Left,
		Right,
	};
}

namespace SceneID
{
	enum ID
	{
		None,
		Title,
		Game,
		Gameover,
		Multiplayer,
	};
}

namespace NetworkScenes
{
	enum ID
	{
		Connect,
		Game,
		Disconnect,
		PreGameLobby,
		RoomSelect,
	};
}

namespace TextureID
{
	enum ID
	{
		BoatMapMain,
		BoatMapOverlay,
		Player,
		Background,
		Button,
		ButtonHighlight,
		Boat,
		Tiles,
		GameWorldTiles,
		Water,
		TitleScreenBG,
		PreGameLobbyBG,
		DisconnectScreenBG,
		GameOverBG,
		Notification,
		CannonBall,
		LoadingAnim,
		Footsteps,
		Octupus,
		Barrel,
		BoatMap,
		GameWorld,
		RoomStatus,
		Smoke,
		Fire,
		Countdown,
		Coin,
		Star,
		CoinOverlay,
		CoinBackground,
		RoomSceneBG,
		Bubble,
		ExplodingBubble,
		LoadedCannonBall,
		Wheel,
		GhostBoat,
		WinScreen,
		LoseScreen,
		BoatShadow,
		Player2,
		ConnectScreen,
		RoomScreen,
		Engine,
		Rudder,
		Vignette,
		GameOver,
		Positions
	};
}
namespace SoundEffectID
{
	enum ID
	{
		EngineStart,
		Engine,
		FixingRudder,
		Footsteps,
		Coin,
		SpendCoin,
		FireCannon,
		Lift,
		DropCannonBall,
		loadSound,
		BackgroundMusic,
		Sploosh,
		Kaboom,
		Error,
		Crash,
		Shovelling,
		Success
	};
}
namespace MusicID
{
	enum ID
	{
		Theme,
	};
}