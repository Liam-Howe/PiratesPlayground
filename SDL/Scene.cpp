#include "stdafx.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Entity.h"

Scene::Scene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadpool)
	: m_sceneManager(&sceneManager)
{
	Entity::resetGlobalID();
}

Scene::~Scene()
{
}

void Scene::requestScenePush(SceneID::ID sceneID)
{
	m_sceneManager->pushScene(sceneID);
}

void Scene::requestScenePop()
{
	m_sceneManager->popScene();
}

void Scene::requestSceneClear()
{
	m_sceneManager->clearScene();
}
