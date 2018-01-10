#include "stdafx.h"
#include "SceneManager.h"
#include "ContentLoader.h"
#include "Entity.h"
#include "AudioManager.h"


SceneManager::SceneManager()
	: m_sceneStack()
	, m_sceneFactory()
	, m_numOfControllers(0)
{
	m_numOfControllers = m_inputHandler.init();
	Constants::MAX_PLAYERS = m_numOfControllers;
}

SceneManager::~SceneManager()
{
}

void SceneManager::loadContent(SDL_Renderer * renderer)
{
	// Load all of the game content, Images, sounds etc

	//// LoadBmpTexture Params(filePath, TextureID, transparent, ColorKey, renderer)
	//SDL_Color colorKey{ 0xFF, 0, 0xFF };	// Removes that color from the image.
	//m_contentLoader.loadBmpTexture("assets/test.bmp", TextureID::Player, true, &colorKey, renderer);

	// LoadPngTexture Params(filePath, TextureID, renderer)

	//m_contentLoader.loadPngTexture("assets/map.png", TextureID::BoatMap, renderer);
	m_contentLoader.loadPngTexture("assets/map1.png", TextureID::BoatMapMain, renderer);
	m_contentLoader.loadPngTexture("assets/map2.png", TextureID::BoatMapOverlay, renderer);

	m_contentLoader.loadPngTexture("assets/button.png",		TextureID::Button, renderer);
	m_contentLoader.loadPngTexture("assets/buttonHL.png",	TextureID::ButtonHighlight, renderer);
	m_contentLoader.loadPngTexture("assets/test2.png",		TextureID::Background, renderer);
	m_contentLoader.loadPngTexture("assets/Boat.png",		TextureID::Boat, renderer);
	m_contentLoader.loadPngTexture("assets/player.png",		TextureID::Player, renderer);
	m_contentLoader.loadPngTexture("assets/tiles.png",		TextureID::Tiles, renderer);
	m_contentLoader.loadPngTexture("assets/GameWorldTiles.png",		TextureID::GameWorldTiles, renderer);
	m_contentLoader.loadPngTexture("assets/water.png",		TextureID::Water, renderer);
	m_contentLoader.loadPngTexture("assets/titleScreen.png",TextureID::TitleScreenBG, renderer);
	m_contentLoader.loadPngTexture("assets/DisconnectedScreen.png", TextureID::DisconnectScreenBG, renderer);
	m_contentLoader.loadPngTexture("assets/PreGameLobby.png", TextureID::PreGameLobbyBG, renderer);
	m_contentLoader.loadPngTexture("assets/RoomScreen.png", TextureID::GameOverBG, renderer);
	m_contentLoader.loadPngTexture("assets/Bubble.png",		TextureID::Notification, renderer);
	m_contentLoader.loadPngTexture("assets/cannonball.png", TextureID::CannonBall, renderer);
	m_contentLoader.loadPngTexture("assets/LoadingAnim.png", TextureID::LoadingAnim, renderer);
	m_contentLoader.loadPngTexture("assets/footsteps.png", TextureID::Footsteps, renderer);
	m_contentLoader.loadPngTexture("assets/octupus.png", TextureID::Octupus, renderer);
	m_contentLoader.loadPngTexture("assets/GameWorld.png", TextureID::GameWorld, renderer);
	m_contentLoader.loadPngTexture("assets/barrel.png", TextureID::Barrel, renderer);
	m_contentLoader.loadPngTexture("assets/Rooms.png", TextureID::RoomStatus, renderer);

	m_contentLoader.loadPngTexture("assets/loadedCannonBall.png", TextureID::LoadedCannonBall, renderer);

	m_contentLoader.loadPngTexture("assets/Steering_Wheel_Anim.png", TextureID::Wheel, renderer);
	m_contentLoader.loadPngTexture("assets/engine.png", TextureID::Engine, renderer);

	m_contentLoader.loadPngTexture("assets/Countdown.png", TextureID::Countdown, renderer);
	m_contentLoader.loadPngTexture("assets/smoke.png", TextureID::Smoke, renderer);
	m_contentLoader.loadPngTexture("assets/fire.png", TextureID::Fire, renderer);
	m_contentLoader.loadPngTexture("assets/coinSheet.png", TextureID::Coin, renderer);
	m_contentLoader.loadPngTexture("assets/coinbar.png", TextureID::CoinOverlay, renderer);
	m_contentLoader.loadPngTexture("assets/coinBackground.png", TextureID::CoinBackground, renderer);
	m_contentLoader.loadPngTexture("assets/star.png", TextureID::Star, renderer);

	m_contentLoader.loadPngTexture("assets/bubbles.png", TextureID::Bubble, renderer);
	m_contentLoader.loadPngTexture("assets/octupusbubbles.png", TextureID::ExplodingBubble, renderer);

	m_contentLoader.loadPngTexture("assets/ghostboat.png", TextureID::GhostBoat, renderer);
	m_contentLoader.loadPngTexture("assets/YouWin.png", TextureID::WinScreen, renderer);
	m_contentLoader.loadPngTexture("assets/YouLose.png", TextureID::LoseScreen, renderer);
	m_contentLoader.loadPngTexture("assets/ConnectScreen.png", TextureID::ConnectScreen, renderer);
	m_contentLoader.loadPngTexture("assets/RoomScreen.png", TextureID::RoomScreen, renderer);

	m_contentLoader.loadPngTexture("assets/BoatShadow.png", TextureID::BoatShadow, renderer);
	m_contentLoader.loadPngTexture("assets/player2.png", TextureID::Player2, renderer);
	m_contentLoader.loadPngTexture("assets/rudder.png", TextureID::Rudder, renderer);
	m_contentLoader.loadPngTexture("assets/Vignette.png", TextureID::Vignette, renderer);
	m_contentLoader.loadPngTexture("assets/GameOver.png", TextureID::GameOver, renderer);
	m_contentLoader.loadPngTexture("assets/Positions.png", TextureID::Positions, renderer);


	m_contentLoader.loadSound("assets/sfx/Fixing_Rudder.wav", SoundEffectID::FixingRudder, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Fire_Cannon.wav", SoundEffectID::FireCannon, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/dropCoin.wav", SoundEffectID::SpendCoin, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Engine_Start.wav", SoundEffectID::EngineStart, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Engine.wav", SoundEffectID::Engine, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Walking.wav", SoundEffectID::Footsteps, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Theme_Tune.wav", MusicID::Theme, ContentLoader::MUSIC);
	m_contentLoader.loadSound("assets/sfx/coin.wav", SoundEffectID::Coin, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Lift_CannonBall.wav", SoundEffectID::Lift, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/metalDrop.wav", SoundEffectID::DropCannonBall, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/loadSound.wav", SoundEffectID::loadSound, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/kaboom.wav", SoundEffectID::Kaboom, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/sploosh.wav", SoundEffectID::Sploosh, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/error.wav", SoundEffectID::Error, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/crash.wav", SoundEffectID::Crash, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/theme.wav", SoundEffectID::BackgroundMusic, ContentLoader::MUSIC);
	m_contentLoader.loadSound("assets/sfx/Shovelling.wav", SoundEffectID::Shovelling, ContentLoader::SFX);
	m_contentLoader.loadSound("assets/sfx/Success.wav", SoundEffectID::Success, ContentLoader::SFX);

	AudioManager::Instance()->init(&m_contentLoader);

}

void SceneManager::unloadContent()
{
	m_contentLoader.cleanUp();
}

bool SceneManager::handleInput(float dt)
{
	if (!m_sceneStack.empty())
	{
		if (m_inputHandler.update(m_event))
		{
			return true;
		}
		if(m_numOfControllers > 0)
			m_sceneStack.back()->handleEvents(&m_inputHandler, dt);
	}
	applyAwaitingChanges();
	return false;
}

void SceneManager::update(float deltaTime)
{
	if (!m_sceneStack.empty())
	{
		m_sceneStack.back()->update(deltaTime);
	}
	applyAwaitingChanges();
}

void SceneManager::render(SDL_Renderer * renderer)
{
	if (!m_sceneStack.empty())
	{
		m_sceneStack.back()->render(renderer);
	}
}

void SceneManager::clearScene()
{
	m_awaitingList.push_back(AwaitingChange(Actions::Clear));
}

void SceneManager::pushScene(SceneID::ID scene)
{
	m_awaitingList.push_back(AwaitingChange(Actions::Push, scene));
}

void SceneManager::popScene()
{
	m_awaitingList.push_back(AwaitingChange(Actions::Pop));
}

size_t SceneManager::getSceneStackSize()
{
	return m_sceneStack.size();
}

int SceneManager::getNumOfJoysticks()
{
	return m_numOfControllers;
}

void SceneManager::applyAwaitingChanges()
{
	for each (AwaitingChange change in m_awaitingList)
	{
		switch (change.m_action)
		{
		case Actions::Push:
			if (!m_sceneStack.empty())
			{
				m_sceneStack.back()->onExit();
				delete m_sceneStack.back();
				m_sceneStack.pop_back();
			}
			m_sceneStack.push_back(addScene(change.m_scene));
			m_sceneStack.back()->onEnter();
			break;

		case Actions::Pop:
			m_sceneStack.back()->onExit();
			delete m_sceneStack.back();
			m_sceneStack.pop_back();
			break;

		case Actions::Clear:
			for (int i = 0; i < m_sceneStack.size(); ++i)
			{
				delete m_sceneStack[i];
			}
			m_sceneStack.clear();
			break;
		}
	}
	m_awaitingList.clear();
}
Scene * SceneManager::addScene(SceneID::ID scene)
{

	Entity::resetGlobalID();
	auto found = m_sceneFactory.find(scene);
	assert(found != m_sceneFactory.end());
	return found->second();
}
SceneManager::AwaitingChange::AwaitingChange(Actions::ID action, SceneID::ID scene)
	: m_action(action)
	, m_scene(scene)
{
}
