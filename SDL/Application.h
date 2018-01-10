#pragma once
#include <SDL.h>
#include "SceneManager.h"

class Application
{
public:
	Application();
	~Application();

	bool initialize(const char*, int, int, int, int, int);
	void cleanUp();
	void loop();

private:
	bool m_quit;
	unsigned int m_lastTime;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	SceneManager m_sceneManager;

	void createScenes();
	void render();
	void update();
	void handleEvents();
};

