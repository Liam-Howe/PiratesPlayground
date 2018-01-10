#include "stdafx.h"
#include "Application.h"
#include "LTimer.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "MultiplayerScene.h"
#include "GameOverScene.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Application::Application()
	: m_quit(false)
	, m_lastTime(LTimer::gameTime())
{
}


Application::~Application()
{
}

bool Application::initialize(const char * title, int xPos, int yPos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//SDL_SetWindowFullscreen(m_window,SDL_WINDOW_FULLSCREEN);
		SDL_SetRenderDrawColor(m_renderer, 75, 150, 75, 255);
	}
	createScenes();         // Creates all game scenes
	m_sceneManager.loadContent(m_renderer);   // Loads all game content
	m_sceneManager.pushScene(SceneID::Title);
	return true;
}

void Application::cleanUp()
{
	m_sceneManager.unloadContent();
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
}

void Application::loop()
{
	LTimer capTimer;
	int frameNum = 0;
	capTimer.start();
	while (!m_quit)
	{
		unsigned int currentTime = LTimer::gameTime();
		float deltaTime = (currentTime - m_lastTime) / 1000.f;
		handleEvents();
		update();
		render();

		if (m_sceneManager.getSceneStackSize() == 0)
		{
			m_quit = true;
		}
	}
}

void Application::createScenes()
{
	m_sceneManager.createScene<TitleScene>(SceneID::Title);
	m_sceneManager.createScene<GameScene>(SceneID::Game);

	m_sceneManager.createScene<MultiplayerScene>(SceneID::Multiplayer);
	m_sceneManager.createScene<GameOverScene>(SceneID::Gameover);
}

void Application::render()
{
	SDL_SetRenderDrawColor(m_renderer, 75, 150, 75, 255);
	SDL_RenderClear(m_renderer);

	m_sceneManager.render(m_renderer);

	SDL_RenderPresent(m_renderer);
}

void Application::update()
{
	unsigned int currentTime = LTimer::gameTime();
	float deltaTime = (currentTime - m_lastTime) / 1000.f;
	m_sceneManager.update(deltaTime);
	m_lastTime = currentTime;
}

void Application::handleEvents()
{
	unsigned int currentTime = LTimer::gameTime();
	float deltaTime = (currentTime - m_lastTime) / 1000.f;
	if (m_sceneManager.handleInput(deltaTime))
	{
		m_quit = true;
	}
}