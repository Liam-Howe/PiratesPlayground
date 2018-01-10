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
#include "CoinSystem.h"
#include "EntityAnimationSystem.h"
#include "CoinBarSystem.h"
class GameScene : public Scene
{
public:
	GameScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadPool);
	~GameScene();

	void handleEvents(InputHandler* inputHandler, float dt) override;
	void update(float deltaTime) override;
	void render(SDL_Renderer * renderer) override;

	void onEnter();
	void onExit();


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
	void setUpSteeringWheel(ContentLoader& contentLoader);
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
	void setupGameOver(ContentLoader & content_loader);

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
	Entity m_vignette;
	vector<Entity> m_notifications;
	Entity m_camera;
	Entity m_dummyNotificationsHolder;
	Entity m_boatMapMain;
	vector<Entity> m_boatMapOverlay;
	Entity m_gameWorldOverlay;
	Entity m_gameOverScreen;
	GameState m_gameState;

	static Entity m_eventHandler;

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
	vector<ParticleManager*>  m_playerFootsteps;

	//obstacle entities
	vector<Entity> m_obstacleEntities;


	vector<Entity*> m_enemyBoats;
	// Sounds
	float m_timer;
	bool m_engineStarted;

	Entity m_boatShadow;
	GameState m_state;


	//coins vector
	std::vector<Entity> m_coins;
	CoinSystem m_coinSystem;
	std::vector<SDL_Rect> m_coinGoalRects;
	vector<Entity> m_coinBars;
	vector<Entity> m_coinBarBackground;


	Vector2 boatParticlePosition;
	Vector2 boatSmokePosition;
	std::vector<Vector2> m_octupusParticlePositions;
	std::vector<Entity> m_loadedCannonEntities;

	Entity m_steeringWheel;
	Entity m_engine;
	Entity m_rudder;
	
};

