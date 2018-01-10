#pragma once
#include <iostream>
#include <string>
#include "SDL.h"
#include "InputHandler.h"
#include "ContentLoader.h"
#include "ThreadPool.h"
#include "Identifiers.h"

class SceneManager;
class Scene
{
public:
	Scene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadpool);
	virtual ~Scene();

	virtual void handleEvents(InputHandler * inputHandler, float dt)= 0;
	virtual void update(float deltaTime) = 0;
	virtual void render(SDL_Renderer * renderer) = 0;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;

protected:
	void requestScenePush(SceneID::ID sceneID);
	void requestScenePop();
	void requestSceneClear();

private:
	SceneManager* m_sceneManager;
};

