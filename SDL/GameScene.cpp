#include "stdafx.h"
#include "GameScene.h"
#include "TileComponent.h"
#include "DummyCannonBallComponent.h"
#include "CollisionComponent.h"
#include "EventListenerComponent.h"
#include "EventHandlerComponent.h"
#include "BoatComponent.h"
#include "FurnaceStationComponent.h"
#include "EngineComponent.h"
#include "CannonStationComponent.h"
#include "NotificationsHolder.h"
#include "CannonBallStationComponent.h"
#include "ParticleComponent.h"
#include "ParticleManager.h"
#include "AliveComponent.h"
#include "CoinComponent.h"
#include "CoinBarComponent.h"
#include "LoadedCannonBallComponent.h"
Entity GameScene::m_eventHandler;
GameScene::GameScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadPool)
	: Scene(sceneManager, contentLoader, threadPool)
	, m_players(Constants::MAX_PLAYERS_PER_SHIP)
	, m_tiles(Constants::MAX_NUM_OF_ROWS, vector<Entity>(Constants::MAX_NUM_OF_COLS))
	, m_gameWorldTiles(Constants::MAX_WORLD_TILES_ROWS, vector<Entity>(Constants::MAX_WORLD_TILES_COLS))
	, m_boatMapOverlay(2)
	,m_coins(Constants::MAX_NUM_OF_COINS),
	m_coinBars(Constants::MAX_PLAYERS_PER_SHIP)
	,m_coinBarBackground(Constants::MAX_PLAYERS_PER_SHIP)
	,m_obstacleEntities(Constants::MAX_ENTITIES),
	m_octupusParticlePositions(Constants::MAX_ENTITIES),
	m_loadedCannonEntities(Constants::NUM_OF_CANNONS)
{
	SDL_Texture * temp = contentLoader.getTexture(TextureID::Footsteps);
	ParticleManager::LoadPresets(temp);
	m_eventHandler.addComponent<EventHandlerComponent>(new EventHandlerComponent(), ComponentTypes::EventHandler);
	setUpSteeringWheel(contentLoader);
	setUpPlayer(contentLoader);
	addAIs(contentLoader);
	setUpGameWorldTiles(contentLoader);
	setupGameWorldOverlay(contentLoader);
	setupObstacleEntities(contentLoader);
	setUpBoat(contentLoader);
	setupBoatMapMain(contentLoader);
	setupLoadedCannonEntities(contentLoader);
	setUpTiles(contentLoader);
	setUpCamera(contentLoader);
	setupCoins(contentLoader);
	
	setupCoinOverlay(contentLoader);
	setupRightSideCannonBalls(contentLoader);
	setupBoatMapOverlay(contentLoader);
	setUpNotifications(contentLoader);
	
	setupLeftSideCannonBalls(contentLoader);
	setupVignette(contentLoader);
	setupGameOver(contentLoader);
	m_cameraSystem.setBoatIndex(m_boat.m_ID);
	m_cameraSystem.setBoatShadowIndex(m_boatShadow.m_ID);
	
	
	
	srand(NULL);
}


void GameScene::setupObstacleEntities(ContentLoader& contentLoader)
{
	vector<pair<Vector2, int>> m_entityData(Constants::MAX_ENTITIES);
	m_entityData = jsonLoader.loadEntities("assets/entities.json");
	for (int i = 0; i < Constants::MAX_ENTITIES; i++)
	{
		m_obstacleEntities[i].addComponent<PositionComponent>(new PositionComponent(Vector2(m_entityData[i].first)), ComponentTypes::Position);
		m_obstacleEntities[i].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(m_entityData[i].second), SDL_Rect{ 0,0,64,64 }, SDL_Point{ 64,64 }, m_obstacleEntities[i].m_ID), ComponentTypes::Sprite);
		m_obstacleEntities[i].addComponent<WorldRenderableComponent>(new WorldRenderableComponent(), ComponentTypes::WorldRenderable);

		m_obstacleEntities[i].addComponent<AliveComponent>(new AliveComponent(m_obstacleEntities[i].m_ID), ComponentTypes::AliveComponent);
		m_obstacleEntities[i].addComponent<CollisionComponent>(new CollisionComponent(SDL_Rect{(int)m_obstacleEntities[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x,
		(int)m_obstacleEntities[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y ,64,64}), ComponentTypes::Collision);
		
		vector<ParticleManager::ParticleSettings> octupusSettingsVec = vector<ParticleManager::ParticleSettings>();
		ParticleManager::ParticleSettings octupusSettings = ParticleManager::ParticleSettings();
		octupusSettings._startingVelocity = 100;
		octupusSettings._endingVelocity = 40;
		octupusSettings._texture = contentLoader.getTexture(TextureID::ExplodingBubble);
		octupusSettings._particleSize = 15;
		octupusSettings._shapeType = Shape::NULL_SHAPE;
		octupusSettings._minTTL = 1;
		octupusSettings._maxTTL = 2;
		octupusSettings._emissionRate = 0.01;
		octupusSettings._velVariation = 5;
		octupusSettings._startingDirection = new Vector2(0, 1);

		octupusSettingsVec.push_back(octupusSettings);
		vector<Vector2 *> particlePos = vector<Vector2*>();

		m_octupusParticlePositions[i] = Vector2(0, 0);
		particlePos.push_back(&m_octupusParticlePositions[i]);
		m_obstacleEntities[i].addComponent<ParticleComponent>(new ParticleComponent(particlePos, octupusSettingsVec), ComponentTypes::Particle);
		m_obstacleEntities[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();
		
		m_particleSys.addEntity(&m_obstacleEntities[i]);
		m_cameraSystem.addEntity(&m_obstacleEntities[i]);
	}
}

void GameScene::setupBoatMapMain(ContentLoader & contentLoader)
{
	m_boatMapMain.addComponent<PositionComponent>(new PositionComponent(Vector2(0, 0), m_boatMapMain.m_ID), ComponentTypes::Position);
	m_boatMapMain.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::BoatMapMain), SDL_Rect{ 0,0,1920,1088 }, SDL_Point{ 1920,1088 }, m_boatMapMain.m_ID), ComponentTypes::Sprite);
	//m_renderSystem.addEntity(&m_boatMapMain);
}



void GameScene::setupBoatMapOverlay(ContentLoader & contentLoader)
{
	m_boatMapOverlay[0].addComponent<PositionComponent>(new PositionComponent(Vector2(416, 118), m_boatMapOverlay[0].m_ID), ComponentTypes::Position);
	m_boatMapOverlay[0].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::BoatMapOverlay), SDL_Rect{ 0,0,1088,77 }, SDL_Point{ 1088,77 }, m_boatMapOverlay[0].m_ID), ComponentTypes::Sprite);
	m_renderSystem.addEntity(&m_boatMapOverlay[0]);
	m_boatMapOverlay[1].addComponent<PositionComponent>(new PositionComponent(Vector2(416, 950), m_boatMapOverlay[1].m_ID), ComponentTypes::Position);
	m_boatMapOverlay[1].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::BoatMapOverlay), SDL_Rect{ 0,0,1088,77 }, SDL_Point{ 1088,77 }, m_boatMapOverlay[0].m_ID), ComponentTypes::Sprite);
	m_renderSystem.addEntity(&m_boatMapOverlay[1]);
}

void GameScene::setupGameWorldOverlay(ContentLoader & contentLoader)
{
	m_gameWorldOverlay.addComponent<PositionComponent>(new PositionComponent(Vector2(0, 0), m_gameWorldOverlay.m_ID), ComponentTypes::Position);
	m_gameWorldOverlay.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::GameWorld), SDL_Rect{ 0,0,1088,32000 }, SDL_Point{ 1088,32000 }, m_gameWorldOverlay.m_ID), ComponentTypes::Sprite);

	m_gameWorldOverlay.addComponent<WorldRenderableComponent>(new WorldRenderableComponent(), ComponentTypes::WorldRenderable);
	m_gameWorldOverlay.getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_texture;
	m_cameraSystem.addEntity(&m_gameWorldOverlay);
	//m_renderSystem.addEntity(&m_gameWorldOverlay);
}

void GameScene::setupCoins(ContentLoader & contentLoader)
{
	std::vector<Vector2> m_coinPositions = jsonLoader.loadCoinData("assets/coins.json");
	for (int i = 0; i < m_coins.size(); i++)
	{
		m_coins[i].addComponent<PositionComponent>(new PositionComponent(m_coinPositions[i]), ComponentTypes::Position);
		m_coins[i].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Coin), SDL_Rect{ 0,0,40,40 }, SDL_Point{ Constants::_Coin_Size,Constants::_Coin_Size }, m_coins[i].m_ID), ComponentTypes::Sprite);
		m_coins[i].addComponent<CoinComponent>(new CoinComponent(m_coins[i].m_ID, SDL_Rect{0,0,Constants::_Coin_Size,Constants::_Coin_Size }),ComponentTypes::CoinComponent);
		m_coins[i].addComponent<VelocityComponent>(new VelocityComponent(Vector2(0, 0), m_coins[i].m_ID), ComponentTypes::Velocity);
		m_coins[i].addComponent<AnimationComponent>(new AnimationComponent(m_coins[i].m_ID, SDL_Point{ 40, 40}, 0.07f, 5), ComponentTypes::Animation);
		vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
		ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
		settings._startingVelocity = 50;
		settings._endingVelocity = 10;
		settings._texture = contentLoader.getTexture(TextureID::Star);
		settings._particleSize = 30;
		settings._offsetFromParent = Vector2(0,0);
		settings._shapeType = Shape::NULL_SHAPE;
		settings._emissionRate = 0.025;
		settings._minTTL = 1;
		settings._maxTTL = 3;
		settings._startingDirection = new Vector2(-0.5, -1);
		settings._velVariation = 1;
		settingsVec.push_back(settings);

		vector<Vector2 *> pos = vector<Vector2*>();
		pos.push_back(&m_coins[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
		m_coins[i].getComponent<CoinComponent>(ComponentTypes::CoinComponent)->m_spawnPosition = m_coins[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position;
		ParticleManager::ParticleSettings coinSettings = ParticleManager::ParticleSettings();
		coinSettings._startingVelocity = 50;
		coinSettings._endingVelocity = 0;
		coinSettings._texture = contentLoader.getTexture(TextureID::Star);
		coinSettings._particleSize = 15;
		coinSettings._offsetFromParent = Vector2(Constants::_Coin_Size/8, Constants::_Coin_Size / 8);
		coinSettings._shapeType = Shape::NULL_SHAPE;
		coinSettings._emissionRate = 0.0015;
		coinSettings._minTTL = 1;
		coinSettings._maxTTL = 1.5;
		coinSettings._velVariation = 10;
		settingsVec.push_back(coinSettings);
		
		pos.push_back(&m_coins[i].getComponent<CoinComponent>(ComponentTypes::CoinComponent)->m_spawnPosition);
		m_coins[i].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);
		m_coins[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();
		m_coins[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1].TurnOff();
		m_coins[i].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
		m_eventHandlerSystem.addListener(Event::EventType::SetSourceRect, m_coins[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventListenerSystem.addEntity(&m_coins[i]);
		m_entityAnimationSystem.addEntity(&m_coins[i]);
		m_particleSys.addEntity(&m_coins[i]);
		m_renderSystem.addEntity(&m_coins[i]);
		m_coinSystem.addEntity(&m_coins[i]);
		m_physicsSystem.addEntity(&m_coins[i]);
	
	}
	m_coinGoalRects.push_back(SDL_Rect{ 800,-100,Constants::TILE_SIZE,Constants::TILE_SIZE });
	m_coinGoalRects.push_back(SDL_Rect{ 1200,-100,Constants::TILE_SIZE,Constants::TILE_SIZE });

}



void GameScene::setupCoinOverlay(ContentLoader & contentLoader)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		if (m_players[i].m_ID <= Constants::MAX_PLAYERS)
		{
			m_coinBars[i].addComponent<PositionComponent>(new PositionComponent(Vector2(m_players[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x, m_players[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y), m_coinBars[i].m_ID), ComponentTypes::Position);
			m_coinBars[i].addComponent<CoinBarComponent>(new CoinBarComponent(m_players[i].m_ID), ComponentTypes::CoinBarComponent);
			m_coinBars[i].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::CoinOverlay), SDL_Rect{ 0,0,96,15 }, SDL_Point{ 96,15 }, m_coinBars[i].m_ID), ComponentTypes::Sprite);
			m_coinBars[i].addComponent<AnimationComponent>(new AnimationComponent(m_coinBars[i].m_ID, SDL_Point{ 40, 40 }, 0.07f, 5), ComponentTypes::Animation);

			m_coinBarBackground[i].addComponent<PositionComponent>(new PositionComponent(Vector2(m_players[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x, m_players[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y), m_coinBarBackground[i].m_ID), ComponentTypes::Position);
			m_coinBarBackground[i].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::CoinBackground), SDL_Rect{ 0,0,106,25 }, SDL_Point{ 106,25 }, m_coinBarBackground[i].m_ID), ComponentTypes::Sprite);
			m_renderSystem.addEntity(&m_coinBarBackground[i]);
			m_renderSystem.addEntity(&m_coinBars[i]);
			m_coinBarSystem.addEntity(&m_coinBars[i]);
		}
	}
}

void GameScene::setupLoadedCannonEntities(ContentLoader & contentLoader)
{
		m_loadedCannonEntities[0].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::LoadedCannonBall), SDL_Rect{ 0,0,32,15 }, SDL_Point{ 32,15 }, m_loadedCannonEntities[0].m_ID), ComponentTypes::Sprite);
		m_loadedCannonEntities[0].addComponent<PositionComponent>(new PositionComponent(Vector2{ 545,798 }, m_loadedCannonEntities[0].m_ID), ComponentTypes::Position);
		m_loadedCannonEntities[0].addComponent<LoadedCannonBallComponent>(new LoadedCannonBallComponent(m_loadedCannonEntities[0].m_ID), ComponentTypes::LoadedCannonComponent);
		m_loadedCannonEntities[0].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;
		
		m_loadedCannonEntities[1].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::LoadedCannonBall), SDL_Rect{ 0,0,32,15 }, SDL_Point{ 32,15 }, m_loadedCannonEntities[1].m_ID), ComponentTypes::Sprite);
		m_loadedCannonEntities[1].addComponent<PositionComponent>(new PositionComponent(Vector2{ 1330,798 }, m_loadedCannonEntities[1].m_ID), ComponentTypes::Position);
		m_loadedCannonEntities[1].addComponent<LoadedCannonBallComponent>(new LoadedCannonBallComponent(m_loadedCannonEntities[1].m_ID), ComponentTypes::LoadedCannonComponent);
		m_loadedCannonEntities[1].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;
		m_renderSystem.addEntity(&m_loadedCannonEntities[0]);
		m_renderSystem.addEntity(&m_loadedCannonEntities[1]);
		
}

void GameScene::setupVignette(ContentLoader & content_loader)
{
	m_vignette.addComponent<SpriteComponent>(new SpriteComponent(content_loader.getTexture(TextureID::Vignette), SDL_Rect{ 0,0,1920,1080 }, SDL_Point{ 1920,1080 }, m_vignette.m_ID), ComponentTypes::Sprite);
	m_vignette.addComponent<PositionComponent>(new PositionComponent(Vector2(0, 0), m_vignette.m_ID), ComponentTypes::Position);
	m_renderSystem.addEntity(&m_vignette);
}

void GameScene::setupGameOver(ContentLoader & content_loader)
{
	m_gameOverScreen.addComponent<SpriteComponent>(new SpriteComponent(content_loader.getTexture(TextureID::GameOver), SDL_Rect{ 0,0,1920,1080 }, SDL_Point{ 1920,1080 }, m_vignette.m_ID), ComponentTypes::Sprite);
	m_gameOverScreen.addComponent<PositionComponent>(new PositionComponent(Vector2(0, 0), m_vignette.m_ID), ComponentTypes::Position);
}



void GameScene::setUpPlayer(ContentLoader& contentLoader)
{
	for (size_t i = 0; i < Constants::MAX_PLAYERS; i++)
	{
		m_players[i].addComponent<PositionComponent>(new PositionComponent(Vector2(820 + (i * 200), 880), m_players[i].m_ID), ComponentTypes::Position);

		m_players[i].addComponent<VelocityComponent>(new VelocityComponent(Vector2(0, 0), m_players[i].m_ID), ComponentTypes::Velocity);
		m_players[i].addComponent<CollisionComponent>(new CollisionComponent(SDL_Rect{ 0,0,Constants::OFFSET_COLLISION.x, Constants::OFFSET_COLLISION.y }), ComponentTypes::Collision);
		m_playerPositions.push_back(&m_players[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
		if (i == 0)
			m_players[i].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Player), SDL_Rect{ 0,0,Constants::playerSize.x,Constants::playerSize.y }, SDL_Point{ Constants::playerSize.x,Constants::playerSize.y }, m_players[i].m_ID), ComponentTypes::Sprite);
		else
			m_players[i].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Player2), SDL_Rect{ 0,0,Constants::playerSize.x,Constants::playerSize.y }, SDL_Point{ Constants::playerSize.x,Constants::playerSize.y }, m_players[i].m_ID), ComponentTypes::Sprite);

	    vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
	    ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
		settings._startingVelocity = 20;
		settings._endingVelocity = 0;
		settings._texture = contentLoader.getTexture(TextureID::Smoke);
		settings._particleSize = 30;
		settings._offsetFromParent = Vector2(Constants::playerSize.x / 5, Constants::playerSize.y / 3);
		settings._shapeType = Shape::NULL_SHAPE;

		settings._emissionRate = 0.5;
		settings._startingDirection = new Vector2(0, -1);

	    settingsVec.push_back(settings);
	    vector<Vector2 *> pos = vector<Vector2*>();

	
	    pos.push_back(&m_players[i].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
	    
		m_players[i].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);
		m_players[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();
		m_playerFootsteps.push_back(&m_players[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0]);

		m_players[i].addComponent<ControlComponent>(new ControlComponent(m_players[i].m_ID, i), ComponentTypes::Control);
		m_players[i].addComponent<AnimationComponent>(new AnimationComponent(m_players[i].m_ID), ComponentTypes::Animation);
		m_players[i].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
		m_players[i].addComponent<PlayerInteractionComponent>(new PlayerInteractionComponent(m_players[i].m_ID), ComponentTypes::PlayerInteraction);
		
		m_eventHandlerSystem.addListener(Event::EventType::StopAnimation,			m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::StartWalkingAnimation,	m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::PickUpCannonBallAnimation, m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::DropCannonBallAnimation, m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::StartStationAnimation,	m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::SetSourceRect,			m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::ChangeAccel,				m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::PlayerRecieveCannonBall, m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::PlayerDropCannonBall,	m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::PickupCoin, m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::SpendCoin, m_players[i].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));

		m_eventListenerSystem.addEntity(&m_players[i]);

		

		m_collisionSystem.addEntity(&m_players[i]);
		m_renderSystem.addEntity(&m_players[i]);
		m_controlSystem.addEntity(&m_players[i]);
		m_physicsSystem.addEntity(&m_players[i]);
		m_animationSystem.addEntity(&m_players[i]);


	}

	m_engine.addComponent<PositionComponent>(new PositionComponent(Vector2(172, 190)), ComponentTypes::Position);
	m_engine.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Engine), SDL_Rect{ 0,0,102,88 }, SDL_Point{ 102,88 }, m_engine.m_ID), ComponentTypes::Sprite);
	m_engine.addComponent<AnimationComponent>(new AnimationComponent(m_engine.m_ID, SDL_Point{ 102, 88 }, 0.07f, 5), ComponentTypes::Animation);
	m_entityAnimationSystem.addEntity(&m_engine);
	m_renderSystem.addEntity(&m_engine);

	m_rudder.addComponent<PositionComponent>(new PositionComponent(Vector2(1653, 214)), ComponentTypes::Position);
	m_rudder.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Rudder), SDL_Rect{ 0,0,86,104 }, SDL_Point{ 86,104 }, m_rudder.m_ID), ComponentTypes::Sprite);
	m_rudder.addComponent<AnimationComponent>(new AnimationComponent(m_rudder.m_ID, SDL_Point{ 86, 104 }, 0.07f, 2), ComponentTypes::Animation);
	m_entityAnimationSystem.addEntity(&m_rudder);
	m_renderSystem.addEntity(&m_rudder);


}


void GameScene::setUpTiles(ContentLoader & contentLoader)
{

	vector<vector<pair<int, int>>> m_tileData(Constants::MAX_NUM_OF_ROWS, vector<pair<int, int>>(Constants::MAX_NUM_OF_COLS));
	m_tileData = jsonLoader.loadTileIds("assets/shipTiles.json");

	for (int i = 0; i < Constants::MAX_NUM_OF_ROWS; i++)
	{
		for (int j = 0; j < Constants::MAX_NUM_OF_COLS; ++j)
		{
			m_tiles[i][j].addComponent<PositionComponent>(new PositionComponent(Vector2((j * Constants::TILE_SIZE) + Constants::OFFSET_COLLISION.x, i * Constants::TILE_SIZE)), ComponentTypes::Position);
			m_tiles[i][j].addComponent<CollisionComponent>(new CollisionComponent(SDL_Rect{ (j * Constants::TILE_SIZE) + Constants::OFFSET_COLLISION.x ,i * Constants::TILE_SIZE ,Constants::TILE_SIZE , Constants::TILE_SIZE }), ComponentTypes::Collision);

			Vector2 glidePos = m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position;

			if (m_tileData[i][j].second != 0)
				m_tiles[i][j].addComponent<TileComponent>(new TileComponent(m_tileData[i][j].first, true), ComponentTypes::Tile);
			else
				m_tiles[i][j].addComponent<TileComponent>(new TileComponent(m_tileData[i][j].first, false), ComponentTypes::Tile);

			if (m_tiles[i][j].getComponent<TileComponent>(ComponentTypes::Tile)->m_tileType == 15)
			{
				m_tiles[i][j].addComponent<WheelStationComponent>(new WheelStationComponent(SDL_Point{ j,i }, &m_tiles[i][j], m_boat.getComponent<PositionComponent>(ComponentTypes::Position), m_boat.getComponent<VelocityComponent>(ComponentTypes::Velocity), 5, glidePos), ComponentTypes::WheelStation);

				m_tiles[i][j].addComponent<StationComponent>(m_tiles[i][j].getComponent<WheelStationComponent>(ComponentTypes::WheelStation), ComponentTypes::Station);
				m_tiles[i][j].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);

				m_eventHandlerSystem.addListener(Event::EventType::MoveShip, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventHandlerSystem.addListener(Event::EventType::DecelerateShip, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventHandlerSystem.addListener(Event::EventType::StopStartWheelInput, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventListenerSystem.addEntity(&m_tiles[i][j]);

				WheelStationComponent * wheel = m_tiles[i][j].getComponent<WheelStationComponent>(ComponentTypes::WheelStation);
				wheel->m_gridCoord = SDL_Point{ j, i };
				wheel->m_boatPosComponent = m_boat.getComponent<PositionComponent>(ComponentTypes::Position);
				wheel->m_stationEntity = &m_tiles[i][j];
				m_stationsSystem.addEntity(&m_tiles[i][j]);
				m_AIControlSystem.m_stations.push_back(wheel);
				AIControlSystem::wheel = wheel;
			}

			else if (m_tiles[i][j].getComponent<TileComponent>(ComponentTypes::Tile)->m_tileType == 17)
			{
				m_tiles[i][j].addComponent<FurnaceStationComponent>(new FurnaceStationComponent(SDL_Point{ j,i }, &m_tiles[i][j], 5, glidePos), ComponentTypes::FurnaceStation);
				m_tiles[i][j].addComponent<StationComponent>(m_tiles[i][j].getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation), ComponentTypes::Station);
				m_tiles[i][j].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);

				m_eventHandlerSystem.addListener(Event::EventType::ChangeCoalInput, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventHandlerSystem.addListener(Event::EventType::ResetButtonNeeded, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventListenerSystem.addEntity(&m_tiles[i][j]);


				vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
				ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
				settings._startingVelocity = 100;
				settings._endingVelocity = 25;
				settings._particleSize = 130;
				settings._offsetFromParent = Vector2(15, -15);
				settings._shapeType = Shape::NULL_SHAPE;
				settings._emissionRate = 0.25;
				settings._minTTL = 3;
				settings._maxTTL = 5;
				settings._startingDirection = new Vector2(0, -1);
				settings._velVariation = 0.3;
				settingsVec.push_back(settings);

				vector<Vector2 *> pos = vector<Vector2*>();

				ParticleManager::ParticleSettings furnaceSmoke = ParticleManager::ParticleSettings();
				furnaceSmoke._startingVelocity = 50;
				furnaceSmoke._endingVelocity = 0;
				furnaceSmoke._particleSize = 3;
				furnaceSmoke._emissionRate = 0.03;
				furnaceSmoke._velVariation = 0.8f;
				furnaceSmoke._startingDirection = new Vector2(0, -1);
				furnaceSmoke._offsetFromParent = Vector2(100, 22);

				std::vector<ParticleManager::ColourLerper> colours;

				ParticleManager::ColourLerper col1;
				col1._colour = SDL_Color{ 255, 255, 255, 255 };
				col1._durationOfColour = 0.2f;
				colours.push_back(col1);

				ParticleManager::ColourLerper col2;
				col2._colour = SDL_Color{ 255, 137, 48, 255 };
				col2._durationOfColour = 1.5f;
				colours.push_back(col2);

				ParticleManager::ColourLerper col3;
				col3._colour = SDL_Color{ 50, 50, 50, 0 };
				col3._durationOfColour = 1.6f;
				colours.push_back(col3);

				furnaceSmoke._coloursToLerp = colours;
				settingsVec.push_back(furnaceSmoke);


				ParticleManager::ParticleSettings smokeSettings = ParticleManager::ParticleSettings();
				smokeSettings._startingVelocity = 50;
				smokeSettings._endingVelocity = 0;
				smokeSettings._particleSize = 70;
				smokeSettings._texture = contentLoader.getTexture(TextureID::Smoke);
				smokeSettings._offsetFromParent = Vector2(30, -15);
				smokeSettings._shapeType = Shape::NULL_SHAPE;
				smokeSettings._emissionRate = 0.25;
				smokeSettings._minTTL = 3;
				smokeSettings._maxTTL = 4;
				smokeSettings._startingDirection = new Vector2(-0.3, -1);
				smokeSettings._velVariation = 0.7;
				settingsVec.push_back(smokeSettings);


				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);

				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);

				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
				m_tiles[i][j].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);

				m_stationsSystem.addEntity(&m_tiles[i][j]);
				m_AIControlSystem.m_stations.push_back(m_tiles[i][j].getComponent<FurnaceStationComponent>(ComponentTypes::FurnaceStation));
				m_tiles[i][j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[2].TurnOff();
				m_particleSys.addEntity(&m_tiles[i][j]);

			}
			else if (m_tiles[i][j].getComponent<TileComponent>(ComponentTypes::Tile)->m_tileType == 21)
			{
				m_cannon = Entity();
				m_cannon.addComponent<PositionComponent>(new PositionComponent(Vector2((j * Constants::TILE_SIZE), i * Constants::TILE_SIZE)), ComponentTypes::Position);

				m_cannons.push_back(&m_cannon);
				m_cannonPositions.push_back(Vector2(j, i));

				vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
				ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
				settings._startingVelocity = 125;
				settings._endingVelocity = 0;
				settings._texture = contentLoader.getTexture(TextureID::Smoke);
				settings._particleSize = 130;
				settings._offsetFromParent = Vector2(-25, -80);
				settings._shapeType = Shape::NULL_SHAPE;
				settings._emissionRate = 0.025;
				settings._startingDirection = new Vector2(0, -1);
				settingsVec.push_back(settings);
				vector<Vector2 *> pos = vector<Vector2*>();
				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
				m_tiles[i][j].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);
				m_tiles[i][j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();

				m_tiles[i][j].addComponent<CannonStationComponent>(new CannonStationComponent(SDL_Point{ j,i }, &m_tiles[i][j], m_boat.getComponent<PositionComponent>(ComponentTypes::Position), 0, glidePos), ComponentTypes::CannonStation);
				m_tiles[i][j].addComponent<StationComponent>(m_tiles[i][j].getComponent<CannonStationComponent>(ComponentTypes::CannonStation), ComponentTypes::Station);
				m_tiles[i][j].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
				m_eventHandlerSystem.addListener(Event::EventType::Loaded, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventHandlerSystem.addListener(Event::EventType::FireCannon, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));

				m_eventListenerSystem.addEntity(&m_tiles[i][j]);
				m_stationsSystem.addEntity(&m_tiles[i][j]);
				m_AIControlSystem.m_stations.push_back(m_tiles[i][j].getComponent<StationComponent>(ComponentTypes::CannonStation));
				m_particleSys.addEntity(&m_tiles[i][j]);



			}
			else if (m_tiles[i][j].getComponent<TileComponent>(ComponentTypes::Tile)->m_tileType == 22)
			{
				m_cannonBallStation = Entity();
				m_cannonBallStation.addComponent<PositionComponent>(new PositionComponent(Vector2((j * Constants::TILE_SIZE), i * Constants::TILE_SIZE)), ComponentTypes::Position);
				m_tiles[i][j].addComponent<CannonBallStationComponent>(new CannonBallStationComponent(SDL_Point{ j,i }, &m_tiles[i][j], m_boat.getComponent<PositionComponent>(ComponentTypes::Position), 5, glidePos), ComponentTypes::CannonBallStation);
				m_tiles[i][j].addComponent<StationComponent>(m_tiles[i][j].getComponent<CannonBallStationComponent>(ComponentTypes::CannonBallStation), ComponentTypes::Station);
				m_tiles[i][j].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
				m_stationsSystem.addEntity(&m_tiles[i][j]);
				m_AIControlSystem.m_stations.push_back(m_tiles[i][j].getComponent<StationComponent>(ComponentTypes::CannonBallStation));
			}
			else if (m_tiles[i][j].getComponent<TileComponent>(ComponentTypes::Tile)->m_tileType == 23)
			{
				m_tiles[i][j].addComponent<EngineComponent>(new EngineComponent(SDL_Point{ j,i }, &m_tiles[i][j], 0, glidePos), ComponentTypes::Engine);
				m_tiles[i][j].addComponent<StationComponent>(m_tiles[i][j].getComponent<EngineComponent>(ComponentTypes::Engine), ComponentTypes::Station);
				m_tiles[i][j].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
				m_eventHandlerSystem.addListener(Event::EventType::GetNextEngineButton, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));


				vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
				ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
				settings._startingVelocity = 50;
				settings._startingVelocity = 10;
				settings._texture = contentLoader.getTexture(TextureID::Smoke);
				settings._particleSize = 130;
				settings._offsetFromParent = Vector2(0, -50);
				settings._shapeType = Shape::NULL_SHAPE;
				settings._emissionRate = 0.25;
				settings._velVariation = 10;
				settingsVec.push_back(settings);
				vector<Vector2 *> pos = vector<Vector2*>();
				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);


				ParticleManager::ParticleSettings blownUpSettings = ParticleManager::_ROCKET_THRUSTER_PRESET;
				blownUpSettings._emissionRate = 0.02;
				blownUpSettings._velVariation = 1.4;
				blownUpSettings._startingDirection = new Vector2(0, -1);
				blownUpSettings._offsetFromParent = Vector2(60, -40);
				settingsVec.push_back(blownUpSettings);
				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
				m_particleSys.addEntity(&m_tiles[i][j]);


				m_tiles[i][j].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);
				m_tiles[i][j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();
				m_engineParticleManager = &m_tiles[i][j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1];

				m_eventListenerSystem.addEntity(&m_tiles[i][j]);
				m_stationsSystem.addEntity(&m_tiles[i][j]);
				m_AIControlSystem.m_stations.push_back(m_tiles[i][j].getComponent<StationComponent>(ComponentTypes::Station));
			}
			else if (m_tiles[i][j].getComponent<TileComponent>(ComponentTypes::Tile)->m_tileType == 24)
			{
				m_tiles[i][j].addComponent<RudderStation>(new RudderStation(SDL_Point{ j,i }, &m_tiles[i][j], 0, glidePos), ComponentTypes::RudderStation);
				m_tiles[i][j].addComponent<StationComponent>(m_tiles[i][j].getComponent<RudderStation>(ComponentTypes::RudderStation), ComponentTypes::Station);
				m_tiles[i][j].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);

				vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
				
				vector<Vector2 *> pos = vector<Vector2*>();
				
				ParticleManager::ParticleSettings blownUpSettings = ParticleManager::_ROCKET_THRUSTER_PRESET;
				blownUpSettings._emissionRate = 0.02;
				blownUpSettings._velVariation = 1.4;
				blownUpSettings._startingDirection = new Vector2(0, -1);
				blownUpSettings._offsetFromParent = Vector2(100, 40);
				settingsVec.push_back(blownUpSettings);
				pos.push_back(&m_tiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);

				m_tiles[i][j].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);
				m_tiles[i][j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();

				m_rudderParticleManager = &m_tiles[i][j].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0];

				m_eventHandlerSystem.addListener(Event::EventType::IncrementRudderPress, m_tiles[i][j].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
				m_eventListenerSystem.addEntity(&m_tiles[i][j]);
				m_stationsSystem.addEntity(&m_tiles[i][j]);
				m_AIControlSystem.m_stations.push_back(m_tiles[i][j].getComponent<StationComponent>(ComponentTypes::RudderStation));
				m_particleSys.addEntity(&m_tiles[i][j]);
			}

		}
	}
}

void GameScene::setUpGameWorldTiles(ContentLoader & contentLoader)
{
	vector<vector<pair<int, int>>> m_tileData(Constants::MAX_WORLD_TILES_ROWS, vector<pair<int, int>>(Constants::MAX_WORLD_TILES_COLS));
	m_tileData = jsonLoader.loadGameWorld("assets/GameWorld.json");

	for (int i = 0; i < Constants::MAX_WORLD_TILES_ROWS; i++)
	{
		for (int j = 0; j < Constants::MAX_WORLD_TILES_COLS; ++j)
		{
			m_gameWorldTiles[i][j].addComponent<PositionComponent>(new PositionComponent(Vector2((j * Constants::TILE_SIZE), (i * Constants::TILE_SIZE))), ComponentTypes::Position);
			m_gameWorldTiles[i][j].addComponent<CollisionComponent>(new CollisionComponent(SDL_Rect{ (j * Constants::TILE_SIZE) ,i * Constants::TILE_SIZE ,Constants::TILE_SIZE , Constants::TILE_SIZE }), ComponentTypes::Collision);

			if (m_tileData[i][j].second == 1)
				m_gameWorldTiles[i][j].addComponent<TileComponent>(new TileComponent(m_tileData[i][j].first, true), ComponentTypes::Tile);
			else 
				m_gameWorldTiles[i][j].addComponent<TileComponent>(new TileComponent(m_tileData[i][j].first, false), ComponentTypes::Tile);
			
			if (m_tileData[i][j].second == 2)
			{
				vector<Vector2> * waypoints = m_boatAISystem.getWaypoints();
				waypoints->push_back(m_gameWorldTiles[i][j].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
			}
		}
	}
}

void GameScene::setUpBoat(ContentLoader& contentLoader)
{
	m_boat.addComponent<PositionComponent>(new PositionComponent(Vector2(500, 31200)), ComponentTypes::Position);
	m_boat.addComponent<VelocityComponent>(new VelocityComponent(Vector2(0, -0.1), m_boat.m_ID, 0.95f), ComponentTypes::Velocity);

	/// Boat Sprite
	m_boat.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Boat), SDL_Rect{ 0, 0,473, 779 }, SDL_Point{ 64, 128 }, m_boat.m_ID), ComponentTypes::Sprite);
	m_boat.addComponent<CollisionComponent>(new CollisionComponent(SDL_Rect{0,0,64,128}),ComponentTypes::Collision);

	
	m_boat.addComponent<WorldRenderableComponent>(new WorldRenderableComponent(), ComponentTypes::WorldRenderable);
	m_boat.addComponent<BoatComponent>(new BoatComponent(m_boat.getComponent<VelocityComponent>(ComponentTypes::Velocity),m_boat.m_ID), ComponentTypes::Boat);


	m_boat.addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);

	m_boatShadow.addComponent<PositionComponent>(new PositionComponent(Vector2(500, 31200)), ComponentTypes::Position);
	m_boatShadow.addComponent<BoatComponent>(new BoatComponent(m_boat.getComponent<VelocityComponent>(ComponentTypes::Velocity), m_boat.m_ID), ComponentTypes::Boat);
	m_boatShadow.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::BoatShadow), SDL_Rect{ 0,0,473,779 }, SDL_Point{ 64,128 }, m_boatShadow.m_ID), ComponentTypes::Sprite);
	m_boatShadow.addComponent<WorldRenderableComponent>(new WorldRenderableComponent(), ComponentTypes::WorldRenderable);

	m_eventHandlerSystem.addListener(Event::EventType::CoalShovelled, m_boat.getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventHandlerSystem.addListener(Event::EventType::Damage, m_boat.getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	
	vector<ParticleManager::ParticleSettings> boatsettingsVec = vector<ParticleManager::ParticleSettings>();
	ParticleManager::ParticleSettings boatsettings = ParticleManager::ParticleSettings();
	boatsettings._startingVelocity = 100;
	boatsettings._endingVelocity = 40;
	boatsettings._texture = contentLoader.getTexture(TextureID::Bubble);
	boatsettings._particleSize = 15;
	boatsettings._offsetFromParent = Vector2(m_boat.getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_size.x/6,m_boat.getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_size.y/2);
	boatsettings._shapeType = Shape::NULL_SHAPE;
	boatsettings._minTTL = 1;
	boatsettings._maxTTL = 2;
	boatsettings._emissionRate = 0.1;
	boatsettings._velVariation = 1;
	boatsettings._startingDirection = new Vector2(0, 1);

	boatsettingsVec.push_back(boatsettings);


	ParticleManager::ParticleSettings boatSmokesettings = ParticleManager::ParticleSettings();
	boatSmokesettings._startingVelocity = 150;
	boatSmokesettings._endingVelocity = 50;
	boatSmokesettings._texture = contentLoader.getTexture(TextureID::Smoke);
	boatSmokesettings._particleSize = 60;
	boatSmokesettings._offsetFromParent = Vector2(0, 0);
	boatSmokesettings._shapeType = Shape::NULL_SHAPE;
	boatSmokesettings._minTTL = 0.1;
	boatSmokesettings._maxTTL = 0.5;
	boatSmokesettings._emissionRate = 0.025;
	boatSmokesettings._velVariation = 0.5;
	boatSmokesettings._startingDirection = new Vector2(0, -1);

	boatsettingsVec.push_back(boatSmokesettings);

	vector<Vector2 *> particlePos = vector<Vector2*>();

	boatParticlePosition = Vector2(0, 0);
	particlePos.push_back(&boatParticlePosition);
	boatSmokePosition = Vector2(0, 0);
	particlePos.push_back(&boatSmokePosition);

	m_boat.addComponent<ParticleComponent>(new ParticleComponent(particlePos, boatsettingsVec), ComponentTypes::Particle);

	
	m_boat.getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1].TurnOff();


	m_boat.getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();

	m_cameraSystem.addEntity(&m_boatShadow);
	m_renderSystem.addEntity(&m_boatShadow);


	m_eventListenerSystem.addEntity(&m_boat);
	m_particleSys.addEntity(&m_boat);
	m_boatSystem.addEntity(&m_boat);
	m_renderSystem.addEntity(&m_boat);
	m_physicsSystem.addEntity(&m_boat);
	m_cameraSystem.addEntity(&m_boat);
	m_worldCollisionSystem.addEntity(&m_boat);
	m_healthSystem.addEntity(&m_boat);

}

void GameScene::setupRightSideCannonBalls(ContentLoader& contentLoader)
{
	Entity * rightDummyEntity = new Entity();
	rightDummyEntity->addComponent<DummyCannonBallComponent>(new DummyCannonBallComponent(), ComponentTypes::DummyCannonBall);
	rightDummyEntity->addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
	m_eventHandlerSystem.addListener(Event::EventType::ProduceCannonBall, rightDummyEntity->getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventHandlerSystem.addListener(Event::EventType::DropCannonBall, rightDummyEntity->getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventHandlerSystem.addListener(Event::EventType::PickUpCannonBall, rightDummyEntity->getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventHandlerSystem.addListener(Event::EventType::LoadCannonBall, rightDummyEntity->getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventListenerSystem.addEntity(rightDummyEntity);

	for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
	{
		Entity* cannonBall = new Entity();

		cannonBall->addComponent<CannonBallComponent>(new CannonBallComponent(false), ComponentTypes::CannonBall);
		cannonBall->addComponent<PositionComponent>(new PositionComponent(Vector2(m_cannonBallStation.getComponent<PositionComponent>(ComponentTypes::Position)->m_position)), ComponentTypes::Position);
		cannonBall->addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::CannonBall), SDL_Rect{ 0,0,Constants::cannonBallSize.x,Constants::cannonBallSize.y }, SDL_Point{ 32,32 }, true), ComponentTypes::Sprite);
		cannonBall->addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
		cannonBall->getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;

		DummyCannonBallComponent *dummyCannonBallComponent = rightDummyEntity->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall);
		dummyCannonBallComponent->addEntity(cannonBall);

		
		m_cannonBallSystem.addEntity(cannonBall);
		m_eventListenerSystem.addEntity(cannonBall);
		m_rightCannonBalls.push_back(cannonBall);
		m_renderSystem.addEntity(cannonBall);
	}

}


void GameScene::addAIs(ContentLoader& contentLoader)
{
	int numAIs = Constants::MAX_PLAYERS_PER_SHIP - Constants::MAX_PLAYERS;
	for (int i = 0; i < numAIs; ++i)
	{
		int index = i + Constants::MAX_PLAYERS;

		m_players[index].addComponent<PositionComponent>(new PositionComponent(Vector2(880 - (200 * index), 880), m_players[index].m_ID), ComponentTypes::Position);
		/*m_players[index].addComponent<PlayerInteractionComponent>(new PlayerInteractionComponent(m_players[index].m_ID), ComponentTypes::PlayerInteraction);
		m_players[index].getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction)->m_coinTally = 50;*/
		m_coinBars[index].addComponent<CoinBarComponent>(new CoinBarComponent(m_players[index].m_ID), ComponentTypes::CoinBarComponent);
		m_players[index].addComponent<VelocityComponent>(new VelocityComponent(Vector2(0, 0), m_players[index].m_ID, 0.02f), ComponentTypes::Velocity);
		m_playerPositions.push_back(&m_players[index].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
		m_players[index].addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Player2),
			SDL_Rect{ 0,0,Constants::playerSize.x,Constants::playerSize.y },
			SDL_Point{ Constants::playerSize.x,Constants::playerSize.y }, m_players[index].m_ID), ComponentTypes::Sprite);


		vector<ParticleManager::ParticleSettings> settingsVec = vector<ParticleManager::ParticleSettings>();
		ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
		settings._startingVelocity = 20;
		settings._endingVelocity = 0;
		settings._texture = contentLoader.getTexture(TextureID::Smoke);
		settings._particleSize = 30;
		settings._offsetFromParent = Vector2(Constants::playerSize.x / 5, Constants::playerSize.y / 3);
		settings._shapeType = Shape::NULL_SHAPE;

		settings._emissionRate = 0.5;
		settings._startingDirection = new Vector2(0, -1);

		settingsVec.push_back(settings);
		vector<Vector2 *> pos = vector<Vector2*>();


		pos.push_back(&m_players[index].getComponent<PositionComponent>(ComponentTypes::Position)->m_position);

		m_players[index].addComponent<ParticleComponent>(new ParticleComponent(pos, settingsVec), ComponentTypes::Particle);
		m_players[index].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TurnOff();
		m_playerFootsteps.push_back(&m_players[index].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0]);

		m_players[index].addComponent<AIControlComponent>(new AIControlComponent(m_players[index].m_ID), ComponentTypes::AIControl);

		m_players[index].addComponent<BoatAIComponent>(new BoatAIComponent(m_players[index].m_ID, &m_boat), ComponentTypes::BoatAI);

		m_players[index].addComponent<AnimationComponent>(new AnimationComponent(m_players[index].m_ID), ComponentTypes::Animation);
		m_players[index].addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
		m_players[index].addComponent<PlayerInteractionComponent>(new PlayerInteractionComponent(), ComponentTypes::PlayerInteraction);

		m_eventHandlerSystem.addListener(Event::EventType::StopAnimation, m_players[index].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::StartWalkingAnimation, m_players[index].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::StartStationAnimation, m_players[index].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::SetSourceRect, m_players[index].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::ChangeAccel, m_players[index].getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));


		m_eventListenerSystem.addEntity(&m_players[index]);
		m_renderSystem.addEntity(&m_players[index]);
		m_physicsSystem.addEntity(&m_players[index]);
		m_animationSystem.addEntity(&m_players[index]); 
		m_AIControlSystem.addEntity(&m_players[index]); 
		m_boatAISystem.addEntity(&m_players[index]);
	}
}

void GameScene::setupLeftSideCannonBalls(ContentLoader & contentLoader)
{
	Entity * leftDummyEntity = new Entity();
	leftDummyEntity->addComponent<DummyCannonBallComponent>(new DummyCannonBallComponent(),ComponentTypes::DummyCannonBall);
	leftDummyEntity->addComponent<EventListenerComponent>(new EventListenerComponent(),ComponentTypes::EventListener);
	m_eventHandlerSystem.addListener(Event::EventType::FireCannonBall, leftDummyEntity->getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventListenerSystem.addEntity(leftDummyEntity);

		for (int i = 0; i < Constants::CANNON_BALL_SIZE; i++)
		{
			Entity* cannonBall = new Entity();

			cannonBall->addComponent<CannonBallComponent>(new CannonBallComponent(false), ComponentTypes::CannonBall);
			cannonBall->addComponent<PositionComponent>(new PositionComponent(Vector2(0,0)), ComponentTypes::Position);
			cannonBall->addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::CannonBall), SDL_Rect{ 0,0,Constants::cannonBallSize.x,Constants::cannonBallSize.y }, SDL_Point{ 32,32 }, cannonBall->m_ID), ComponentTypes::Sprite);
			cannonBall->addComponent<WorldRenderableComponent>(new WorldRenderableComponent(),ComponentTypes::WorldRenderable);
			cannonBall->addComponent<CollisionComponent>(new CollisionComponent(SDL_Rect{ 0,0,64,64 }), ComponentTypes::Collision);
			cannonBall->getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;
			cannonBall->addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);

			DummyCannonBallComponent *dummyCannonBallComponent = leftDummyEntity->getComponent<DummyCannonBallComponent>(ComponentTypes::DummyCannonBall);
			dummyCannonBallComponent->addEntity(cannonBall);



			m_cannonBallSystem.addEntity(cannonBall);
			m_eventListenerSystem.addEntity(cannonBall);
			m_leftCannonBalls.push_back(cannonBall);
			m_cameraSystem.addEntity(cannonBall);
		}	
}


void GameScene::setUpCamera(ContentLoader& contentLoader)
{
	m_camera = Entity();
	m_camera.addComponent<PositionComponent>(new PositionComponent(Vector2(584, 0)), ComponentTypes::Position);
	m_camera.addComponent<CameraComponent>(new CameraComponent(), ComponentTypes::Camera);
}

void GameScene::setUpSteeringWheel(ContentLoader& contentLoader)
{
	m_steeringWheel.addComponent<PositionComponent>(new PositionComponent(Vector2(895, 780), m_steeringWheel.m_ID), ComponentTypes::Position);
	m_steeringWheel.addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Wheel), SDL_Rect{ 0,0,128,128 }, SDL_Point{ 128,128 }, m_steeringWheel.m_ID, SDL_Point{ 64,64 }), ComponentTypes::Sprite);
	m_renderSystem.addEntity(&m_steeringWheel);
	WheelStationComponent::m_wheel = &m_steeringWheel;
}

void GameScene::setUpNotifications(ContentLoader & contentLoader)
{
	m_notifications = vector<Entity>(); 
	m_dummyNotificationsHolder.addComponent<NotificationHolderComponent>(new NotificationHolderComponent(), ComponentTypes::NotificationHolder);
	m_dummyNotificationsHolder.addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
	m_eventHandlerSystem.addListener(Event::EventType::AddButtonChanged, m_dummyNotificationsHolder.getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventListenerSystem.addEntity(&m_dummyNotificationsHolder);

	for (int i = 0; i < Constants::MAX_PLAYERS; ++i)
	{
		m_notifications.push_back(Entity());
	}

	for (int i = 0; i < m_notifications.size(); ++i)
	{
		m_notifications.at(i).addComponent<PositionComponent>(new PositionComponent(Vector2(1270, 240)), ComponentTypes::Position);

		m_notifications.at(i).addComponent<SpriteComponent>(new SpriteComponent(contentLoader.getTexture(TextureID::Notification), SDL_Rect{ 0, 0, 200, 145 }, SDL_Point{ 100, 73 }), ComponentTypes::Sprite);

		m_notifications.at(i).addComponent<EventListenerComponent>(new EventListenerComponent(), ComponentTypes::EventListener);
		m_eventHandlerSystem.addListener(Event::EventType::DsiplayNotification,			m_notifications.at(i).getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::HideNotification,			m_notifications.at(i).getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::ChangeNotificationIcon,		m_notifications.at(i).getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::SetNextWheelAnim,			m_notifications.at(i).getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventHandlerSystem.addListener(Event::EventType::ResetWheelAnim,				m_notifications.at(i).getComponent<EventListenerComponent>(ComponentTypes::EventListener), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
		m_eventListenerSystem.addEntity(&m_notifications.at(i));
		m_notifications.at(i).addComponent<NotificationComponent>(new NotificationComponent(m_notifications.at(i).getComponent<PositionComponent>(ComponentTypes::Position), m_notifications.at(i).getComponent<SpriteComponent>(ComponentTypes::Sprite), m_players[i].m_ID), ComponentTypes::Notification);

		//m_renderSystem.addEntity(&m_notifications.at(i));
		m_dummyNotificationsHolder.getComponent<NotificationHolderComponent>(ComponentTypes::NotificationHolder)->m_notificationComponents.push_back(m_notifications.at(i).getComponent<NotificationComponent>(ComponentTypes::Notification));
	}
}

GameScene::~GameScene()
{
}

void GameScene::handleEvents(InputHandler * inputHandler, float dt)
{
	switch (m_gameState)
	{
	case Playing:
		for (size_t i = 0; i < Constants::MAX_PLAYERS; i++)
		{
			m_stationsSystem.update(dt, inputHandler, m_players[i].getComponent<ControlComponent>(ComponentTypes::Control), m_players[i].getComponent<AnimationComponent>(ComponentTypes::Animation), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler), &m_camera, m_playerPositions[i], m_playerPositions, m_players[i].getComponent<PlayerInteractionComponent>(ComponentTypes::PlayerInteraction), &m_steeringWheel);
		}

		m_controlSystem.handleInput(inputHandler, m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler), &m_stationsSystem, m_loadedCannonEntities, m_boat);

		
		break;
	case EndScreen:
		for (size_t i = 0; i < Constants::MAX_PLAYERS; i++)
		{
			if (inputHandler->isButtonPressed(m_players[i].getComponent<ControlComponent>(ComponentTypes::Control)->m_ID, JoyStickButtonID::Button_A))
			{
				requestScenePush(SceneID::Title);
			}
		}
		break;
	default: ;
	}



}

void GameScene::update(float deltaTime)
{
	if (m_timer < 1.0)
	{
		m_timer += deltaTime;
	}
	
	if (deltaTime > 0.1f)
		return;

	if(m_engineParticleManager != nullptr)
	{
		if (BoatComponent::m_health == 1)
		{
			m_engineParticleManager->TurnOff();
			m_engine.getComponent<AnimationComponent>(ComponentTypes::Animation)->m_shouldUpdateAnimation = true;
		}
		else
		{
			m_engine.getComponent<AnimationComponent>(ComponentTypes::Animation)->m_shouldUpdateAnimation = false;
			m_engineParticleManager->TurnOn();
			m_engineParticleManager->_emissionRate =  BoatComponent::m_health * 0.1f;
			
			if (m_engineParticleManager->_emissionRate < 0.01)
				m_engineParticleManager->_emissionRate = 0.01f;
		}
	}
	
	if(m_rudderParticleManager != nullptr)
	{
		if (WheelStationComponent::m_canTakeInput)
		{
			m_rudderParticleManager->TurnOff();
			m_rudder.getComponent<AnimationComponent>(ComponentTypes::Animation)->m_shouldUpdateAnimation = true;
		}
		else
		{
			m_rudder.getComponent<AnimationComponent>(ComponentTypes::Animation)->m_shouldUpdateAnimation = false;
			m_rudderParticleManager->TurnOn();
			m_engineParticleManager->_emissionRate = 0.01f;
		}

	}
	
	m_eventHandlerSystem.update(m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_eventListenerSystem.update(m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	
	m_cameraSystem.SetScreenPosition(&m_camera);
	m_cameraSystem.Update(&m_camera);

	m_physicsSystem.update(deltaTime);
	m_boatSystem.update(deltaTime, m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_npcBoatsSystem.update();


	boatParticlePosition = Vector2{ (float)m_boat.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable)->m_screenPos.x,(float)m_boat.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable)->m_screenPos.y };
	boatSmokePosition = Vector2{ (float)m_boat.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable)->m_screenPos.x,(float)m_boat.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable)->m_screenPos.y };

	m_boat.getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0]._position = boatParticlePosition;
	m_boat.getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1]._position = boatSmokePosition;


	for (int i = 0; i < m_obstacleEntities.size(); i++)
	{
		m_octupusParticlePositions[i] = Vector2{ (float)m_obstacleEntities[i].getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable)->m_screenPos.x,(float)m_obstacleEntities[i].getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable)->m_screenPos.y };
		m_obstacleEntities[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0]._position = m_octupusParticlePositions[i];
	}
	
	
	m_cannonBallSystem.updateRightHandSide(m_playerPositions, m_cannonPositions, m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler), m_cannonBallStation.getComponent<PositionComponent>(ComponentTypes::Position)->m_position);

	m_cannonBallSystem.updateLeftHandSide(Vector2{m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x + m_boat.getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_size.x / 2, m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y },deltaTime);
	
	m_healthSystem.update(m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));

	m_coinSystem.update(m_players,m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler), m_coinGoalRects);
	m_coinBarSystem.update(m_players, m_coinBarBackground);
	m_particleSys.update(deltaTime);

	for (int i = 0; i < m_playerFootsteps.size(); ++i)
		m_playerFootsteps[i]->update(deltaTime);

	m_boatAISystem.update(deltaTime, &m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position);
	

	if (m_worldCollisionSystem.handleCollisions(m_gameWorldTiles,m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler)) == 1)
	{

			
			for (int i = 0; i < m_coins.size(); i++)
			{
				m_coins[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[0].TimedTurnOn(0.5f);
				m_coins[i].getComponent<ParticleComponent>(ComponentTypes::Particle)->m_particleManagers[1].TimedTurnOn(0.5f);
				m_coins[i].getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_isAlive = false;
				AudioManager::Instance()->playSound(SoundEffectID::Coin, 3);
			}

		m_gameState = EndScreen;
		
		for (size_t i = 0; i < Constants::MAX_PLAYERS; i++)
		{
			m_players[i].getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_velocity = Vector2(0, 0);
			m_players[i].getComponent<VelocityComponent>(ComponentTypes::Velocity)->m_accel = Vector2(0, 0);
			m_players[i].getComponent<AnimationComponent>(ComponentTypes::Animation)->m_speedScalar = 0;
		}
	}

	// This does the Shadow Scalar
	float percentageAcross = m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x / (960 - 64);
	float maxDistFromBoat = 25;
	float xPos = (-maxDistFromBoat + (percentageAcross * maxDistFromBoat * 2)) + m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x;
	m_boatShadow.getComponent<PositionComponent>(ComponentTypes::Position)->m_position = Vector2(xPos,
																				   m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y);
	m_boatShadow.getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation = m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_eulerRotation;
	

	m_worldCollisionSystem.entityColision(m_obstacleEntities,m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	
	m_cannonBallSystem.updateEntityCollsionBoxes(m_obstacleEntities,Vector2{ m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.x + m_boat.getComponent<SpriteComponent>(ComponentTypes::Sprite)->m_size.x / 2, m_boat.getComponent<PositionComponent>(ComponentTypes::Position)->m_position.y },m_enemyBoats, m_gameWorldTiles,m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));

	m_AIControlSystem.update(deltaTime, &m_stationsSystem, m_boat.getComponent<BoatComponent>(ComponentTypes::Boat), m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler), &m_tiles);

	m_collisionSystem.handleCollisions(m_tiles);
	m_worldCollisionSystem.boatOnboatCollision();
	m_animationSystem.update(deltaTime, m_eventHandler.getComponent<EventHandlerComponent>(ComponentTypes::EventHandler));
	m_entityAnimationSystem.update(deltaTime);
}

void GameScene::render(SDL_Renderer* renderer)
{
	m_renderSystem.renderImage(renderer, m_gameWorldOverlay.getComponent<SpriteComponent>(ComponentTypes::Sprite), m_gameWorldOverlay.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_gameWorldOverlay.getComponent<PositionComponent>(ComponentTypes::Position));

	for (int i = 0; i < m_obstacleEntities.size(); ++i)
	{
		m_renderSystem.renderImage(renderer, m_obstacleEntities[i].getComponent<SpriteComponent>(ComponentTypes::Sprite), m_obstacleEntities[i].getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_obstacleEntities[i].getComponent<PositionComponent>(ComponentTypes::Position));
	}
	for (int i = 0; i < m_leftCannonBalls.size(); ++i)
	{
		m_renderSystem.renderImage(renderer, m_leftCannonBalls[i]->getComponent<SpriteComponent>(ComponentTypes::Sprite), m_leftCannonBalls[i]->getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_leftCannonBalls[i]->getComponent<PositionComponent>(ComponentTypes::Position));
	}
	m_renderSystem.renderImage(renderer, m_boat.getComponent<SpriteComponent>(ComponentTypes::Sprite), m_boat.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_boat.getComponent<PositionComponent>(ComponentTypes::Position));
	m_renderSystem.renderImage(renderer, m_boatMapMain.getComponent<SpriteComponent>(ComponentTypes::Sprite), m_boatMapMain.getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_boatMapMain.getComponent<PositionComponent>(ComponentTypes::Position));


	for (int i = 0; i < m_playerFootsteps.size(); ++i)
	{
		m_playerFootsteps[i]->render(renderer);
	}
	
	/*for (int i = 0; i < m_rightCannonBalls.size(); ++i)
	{
		m_renderSystem.renderImage(renderer, m_rightCannonBalls[i]->getComponent<SpriteComponent>(ComponentTypes::Sprite), m_rightCannonBalls[i]->getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_rightCannonBalls[i]->getComponent<PositionComponent>(ComponentTypes::Position));
	}*/
	
	m_renderSystem.render(renderer);
	
	

	m_particleSys.render(renderer);
	for (int i = 0; i < m_notifications.size(); ++i)
	{
		m_renderSystem.renderImage(renderer, m_notifications[i].getComponent<SpriteComponent>(ComponentTypes::Sprite), m_notifications[i].getComponent<WorldRenderableComponent>(ComponentTypes::WorldRenderable), m_notifications[i].getComponent<PositionComponent>(ComponentTypes::Position));
	}

	if(m_gameState == EndScreen)	
		m_renderSystem.renderImage(renderer, m_gameOverScreen.getComponent<SpriteComponent>(ComponentTypes::Sprite), nullptr, m_gameOverScreen.getComponent<PositionComponent>(ComponentTypes::Position));
}

void GameScene::onEnter()
{
	AudioManager::Instance()->playMusic(SoundEffectID::BackgroundMusic, -1);
}

void GameScene::onExit()
{
	AudioManager::Instance()->stopAllSounds();
	AudioManager::Instance()->stopMusic();
}
