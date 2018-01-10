#pragma once
#include "Scene.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "ControlSystem.h"
#include "PhysicsSystem.h"
#include "CameraSystem.h"
#include "StationsSystem.h"
#include "CollisionSystem.h"
#include "AnimationSystem.h"
#include "JsonLoader.h"
#include "EventListenerSystem.h"
#include "EventHandlerSystem.h"
#include "BoatSystem.h"
#include "CannonBallSystem.h"
#include "ParticleSystem.h"
#include "WorldCollisionSystem.h"
#include "BoatAISystem.h"
#include "AIControlSystem.h"
#include "NPCBoatsSystem.h"
#include "HealthSystem.h"
#include "PacketData.h"
#include "NetworkSystem.h"
#include "Client.h"
#include "CoinSystem.h"
#include "CoinBarSystem.h"
#include "EntityAnimationSystem.h"

class MultiplayerScene : public Scene
{
public:
	MultiplayerScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadPool);
	~MultiplayerScene();

	void init();
	void initConnectScene();
	void initLobby();
	void initGameScene();
	void initDisconnectScene();
	void initRoomSelectScene();

	void handleEvents(InputHandler * inputHandler, float dt) override;
	void handleEventsConnect(InputHandler * inputHandler);
	void handleEventsLobby(InputHandler * inputHandler);
	void handleEventsGame(InputHandler * inputHandler, float dt);
	void handleEventsDisconnect(InputHandler * inputHandler);
	void handleEventsRoomSelect(InputHandler * inputHandler);

	void update(float deltaTime);
	void updateConnect(float deltaTime);
	void updateLobby(float deltaTime);
	void updateGame(float deltaTime);
	void updateDisconnect(float deltaTime);
	void updateRoomSelect(float deltaTime);

	void render(SDL_Renderer * renderer);
	void renderConnect(SDL_Renderer * renderer);
	void renderLobby(SDL_Renderer * renderer);
	void renderGame(SDL_Renderer * renderer);
	void renderDisconnect(SDL_Renderer * renderer);
	void renderRoomSelect(SDL_Renderer * renderer);

	void onEnter();
	void onExit();

	void handlePacket(Uint8& packetType, PacketData& packetData, Client* client);
	void updateRoom(PacketData pData);
private:

	enum GameState
	{
		Playing,
		EndScreen
	};

	void setUpPlayer(ContentLoader& contentLoader);
	void setUpTiles(ContentLoader& contentLoader);
	void setUpGameWorldTiles(ContentLoader & contentLoader);
	void setUpBoat(ContentLoader& contentLoader);
	void setUpCamera(ContentLoader& contentLoader);
	void setUpNotifications(ContentLoader& contentLoader);
	void addAIs(ContentLoader& pContentLoader);
	void setupRightSideCannonBalls(ContentLoader& contentLoader);
	void setupLeftSideCannonBalls(ContentLoader& contentLoader);
	void setupObstacleEntities(ContentLoader& contentLoader);
	void setupBoatMapMain(ContentLoader& contentLoader);
	void setupBoatMapOverlay(ContentLoader& contentLoader);
	void setupGameWorldOverlay(ContentLoader & contentLoader);
	void setupCoins(ContentLoader & contentLoader);
	void setupCoinOverlay(ContentLoader & contentLoader);
	void setupLoadedCannonEntities(ContentLoader & contentLoader);
	void setupVignette(ContentLoader & content_loader);
	CannonBallSystem m_cannonBallSystem;
	SDL_Texture * m_texture;
	RenderSystem m_renderSystem;
	ControlSystem m_controlSystem;
	PhysicsSystem m_physicsSystem;
	CameraSystem m_cameraSystem;
	CollisionSystem m_collisionSystem;
	WorldCollisionSystem m_worldCollisionSystem;
	EntityAnimationSystem m_entityAnimationSystem;

	EventListenerSystem m_eventListenerSystem;
	EventHandlerSystem m_eventHandlerSystem;
	BoatSystem m_boatSystem;
	StationsSystem m_stationsSystem;
	AnimationSystem m_animationSystem;
	ParticleSystem m_particleSys;
	BoatAISystem m_boatAISystem;
	AIControlSystem m_AIControlSystem;
	NPCBoatsSystem m_npcBoatsSystem;
	HealthSystem m_healthSystem;
	CoinBarSystem m_coinBarSystem;
	vector<Entity> m_players;
	vector<vector<Entity>> m_tiles;
	vector<vector<Entity>> m_gameWorldTiles;
	Entity m_levelLoader;
	JsonLoader jsonLoader;
	Entity m_boat;
	vector<Entity> m_notifications;
	Entity m_camera;
	Entity m_dummyNotificationsHolder;
	Entity m_boatMapMain;
	vector<Entity> m_boatMapOverlay;
	Entity m_gameWorldOverlay;
	Entity m_gameOverScreen;
	static Entity m_eventHandler;

	GameState m_gameState;

	//Right hand side Cannon balls
	Entity m_cannonBallStation;
	vector<Entity*> m_rightCannonBalls;
	Entity m_cannon;
	vector<Entity*> m_cannons;
	vector<Vector2> m_cannonPositions;
	vector<Vector2*> m_playerPositions;
	//left hand side cannon balls
	vector<Entity*> m_leftCannonBalls;

	ParticleManager * m_engineParticleManager;
	ParticleManager * m_rudderParticleManager;

	//obstacle entities
	vector<Entity> m_obstacleEntities;
	Entity m_vignette;


	vector<Entity*> m_enemyBoats;
	// Sounds
	float m_timer;
	bool m_engineStarted;


	//coins vector
	std::vector<Entity> m_coins;
	CoinSystem m_coinSystem;
	std::vector<SDL_Rect> m_coinGoalRects;

	vector<Entity> m_coinBars;
	vector<Entity> m_coinBarBackground;



	const Uint32 UPDATETICKS = 100;
	const Uint32 ONESECONDTICK = 1000;
	Uint32 m_countDownTicks;
	Uint32 m_startTicks;
	std::vector<Entity> m_remotePlayers;
	Uint8 m_maxPlayersExLocal;
	ContentLoader& m_contentLoader;

	SDL_Texture* m_btnTexture;
	SDL_Texture* m_btnHighlightTexture;
	SDL_Texture* m_bgTexture;
	Entity m_loadingAnimation;
	

	Client m_client;

	bool m_flag;
	int m_currNetworkScene;
	float m_buttonResetTimer;
	const float REQUIRED_BUTTON_WAIT_TIME = 0.2;

	// ANIMATION stuff
	float m_animationTimer;
	const int FRAME_HEIGHT = 100;
	const int NUM_OF_FRAMES = 7;
	Uint8 m_requestChangeScene;

	//Room Lobby
	bool m_host;
	int m_myID;
	int m_numOfClientsInRoom;
	std::vector<Uint8> m_numberOfRooms;

	//Game Scene
	bool m_countdownFinished;
	
	Entity m_countdownAnimation;
	int m_countdownCounter;
	int m_updateCounter;
	void updateAll(float deltaTime);
	void setupGameOver(ContentLoader& content_loader);
	void setUpRemotePlayers(ContentLoader& contentLoader);
	void setUpSteeringWheel(ContentLoader& contentLoader);
	NetworkSystem m_networkSystem;

	vector<ParticleManager*>  m_playerFootsteps;
	
	int m_packetID;
	
	vector<int> m_remotePlayerIDs;
	// Sounds
	Vector2 boatParticlePosition;
	Vector2 boatSmokePosition;
	std::vector<Vector2> m_octupusParticlePositions;
	std::vector<Entity> m_loadedCannonEntities;
	

	Entity m_boatShadow;
	Entity m_steeringWheel;
	Entity m_engine;
	Entity m_rudder;
	Entity m_result;
	bool m_requestFinishingPosition;
};
