#pragma once
#include "Scene.h"
#include <assert.h>
#include <vector>
#include <map>
#include <functional>
#include "Identifiers.h"
#include "ThreadPool.h"
#include "ContentLoader.h"
#include "Constants.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void loadContent(SDL_Renderer* renderer);				// Load game content
	void unloadContent();									// Unload game content

	bool handleInput(float dt);										// Handle events for the current scene
	void update(float deltaTime);							// Update the current scene
	void render(SDL_Renderer * renderer);					// Render all scenes on the stack

	void clearScene();										// Change scene
	void pushScene(SceneID::ID scene);						// Push scene
	void popScene();										// Pop scene
	size_t getSceneStackSize();								// Size of Scene Stack
	int getNumOfJoysticks();
	template <typename T> void createScene(SceneID::ID scene);

private:
	struct AwaitingChange
	{
		AwaitingChange(Actions::ID action, SceneID::ID scene = SceneID::None);
		Actions::ID m_action;
		SceneID::ID m_scene;
	};
	void applyAwaitingChanges();
	std::vector<AwaitingChange> m_awaitingList;
	Scene* addScene(SceneID::ID scene);								// Adds scene to the stack

	SDL_Event m_event;
	InputHandler m_inputHandler;									// Handles Input
	ContentLoader m_contentLoader;									// Holds textures, SFX, music
	ThreadPool m_threadpool;
	std::vector<Scene*> m_sceneStack;								// Vector or current scenes
	std::map<SceneID::ID, std::function<Scene*()>> m_sceneFactory;	// Map holds all Scenes, invokes createScene when map is called

	SceneManager(const SceneManager& other) = delete;				// Non construction-copyable
	SceneManager& operator=(const SceneManager&) = delete;			// Non copyable
	int m_numOfControllers;
};

template <typename T> void SceneManager::createScene(SceneID::ID sceneID)
{

	Entity::resetGlobalID();
	m_sceneFactory[sceneID] = [this]()
	{
		return new T(*this, m_contentLoader, m_threadpool);
	};
}
