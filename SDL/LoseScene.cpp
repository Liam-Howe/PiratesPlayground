#include "stdafx.h"
#include "LoseScene.h"
#include "Entity.h"
#include "AudioManager.h"

LoseScene::LoseScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadPool)
	: Scene(sceneManager, contentLoader, threadPool)
{
	HierarchyMenu::Instance()->ClearMenu();
	HierarchyMenu::Instance()->CreateBackgroundTexture(SDL_Rect{ 0, 0, 1920, 1080 }, SDL_Rect{ 0, 0, 1920, 1080 }, contentLoader.getTexture(TextureID::LoseScreen));

	HierarchyMenu::Instance()->AddPrimaryButton(SDL_Rect{ 735, 700 , 450, 100 }, SDL_Rect{ 0, 500, 450, 100 }, 0, contentLoader.getTexture(TextureID::Button));
	HierarchyMenu::Instance()->SetButtonHighlight(SDL_Rect{ 0, 0, 50, 50 }, SDL_Rect{ 0, 0, 450, 100 }, contentLoader.getTexture(TextureID::ButtonHighlight));

	Entity::resetGlobalID();
}

LoseScene::~LoseScene()
{
}

void LoseScene::handleEvents(InputHandler * inputHandler)
{
	Vector2 temp = inputHandler->getLeftAnalogAxis(0);
	if (m_buttonResetTimer > REQUIRED_BUTTON_WAIT_TIME)
	{
		// MOVE LEFT ANALOGUE STICK RIGHT
		if (temp.x > 0.5)
		{
			HierarchyMenu::Instance()->ChangeSelectedButton(1);
			m_buttonResetTimer = 0;
		}
		// MOVE LEFT ANALOGUE STICK LEFT
		else if (temp.x < -0.5)
		{
			HierarchyMenu::Instance()->ChangeSelectedButton(-1);
			m_buttonResetTimer = 0;
		}
	}

	if (inputHandler->isButtonPressed(JoyStickID::One, JoyStickButtonID::Button_A))
	{
		HierarchyMenu::Instance()->SelectButton();

		SDL_Point* numReturned = HierarchyMenu::Instance()->getButtonClicked();
		if (numReturned != nullptr)
		{
			switch (numReturned->x)
			{
			case 0:
				requestScenePush(SceneID::Title);
				break;
			}
		}
	}
}

void LoseScene::update(float deltaTime)
{
	m_buttonResetTimer += deltaTime;


}

void LoseScene::render(SDL_Renderer* renderer)
{
	HierarchyMenu::Instance()->Draw(renderer);
}

void LoseScene::onEnter()
{
	AudioManager::Instance()->playMusic(MusicID::Theme, -1);
}

void LoseScene::onExit()
{
	AudioManager::Instance()->stopMusic();
}