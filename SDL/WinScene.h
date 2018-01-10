#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "HierarchyMenu.h"

class WinScene : public Scene
{
public:
	WinScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadpool);
	~WinScene();

	void handleEvents(InputHandler * inputHandler);
	void update(float deltaTime);
	void render(SDL_Renderer * renderer);
	void onEnter();
	void onExit();

private:

	float m_buttonResetTimer;
	const float REQUIRED_BUTTON_WAIT_TIME = 0.2;
};