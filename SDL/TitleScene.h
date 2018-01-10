#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "HierarchyMenu.h"

class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadpool);
	~TitleScene();

	void handleEvents(InputHandler * inputHandler, float dt) override;
	void update(float deltaTime);
	void render(SDL_Renderer * renderer);
	void onEnter();
	void onExit();

private:
	
	float m_buttonResetTimer;
	const float REQUIRED_BUTTON_WAIT_TIME = 0.2;
};