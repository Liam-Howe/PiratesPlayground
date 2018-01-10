#include "stdafx.h"
#include "TitleScene.h"
#include "Entity.h"
#include "AudioManager.h"

TitleScene::TitleScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadPool)
	: Scene(sceneManager, contentLoader, threadPool)
{
	HierarchyMenu::Instance()->ClearMenu();
	HierarchyMenu::Instance()->CreateBackgroundTexture(SDL_Rect{ 0, 0, 1920, 1080 }, SDL_Rect{ 0, 0, 1920, 1080 }, contentLoader.getTexture(TextureID::TitleScreenBG));

	HierarchyMenu::Instance()->AddPrimaryButton(SDL_Rect{ 53, 230 , 623, 137 }, SDL_Rect{ 0, 0, 450, 100 }, 0, contentLoader.getTexture(TextureID::Button));
	HierarchyMenu::Instance()->AddPrimaryButton(SDL_Rect{ 53, 660 , 623, 137 }, SDL_Rect{ 0, 100, 450, 100 }, 1, contentLoader.getTexture(TextureID::Button));

	HierarchyMenu::Instance()->AddSecondaryButton(contentLoader.getTexture(TextureID::Button), SDL_Rect{ 1489, 789 , 417, 103 }, SDL_Rect{ 0, 300, 450, 100 }, 0, 0);
	HierarchyMenu::Instance()->AddSecondaryButton(contentLoader.getTexture(TextureID::Button), SDL_Rect{ 1489, 940 , 417, 103 }, SDL_Rect{ 0, 200, 450, 100 }, 0, 1);

	HierarchyMenu::Instance()->SetButtonHighlight(SDL_Rect{ 0, 0, 50, 50 }, SDL_Rect{ 0, 0, 450, 100 }, contentLoader.getTexture(TextureID::ButtonHighlight));
	HierarchyMenu::Instance()->SetButtonHighlight(SDL_Rect{ 0, 0, 50, 50 }, SDL_Rect{ 0, 0, 450, 100 }, contentLoader.getTexture(TextureID::ButtonHighlight));
	
	
	
	Entity::resetGlobalID();
}

TitleScene::~TitleScene()
{
}

void TitleScene::handleEvents(InputHandler * inputHandler, float dt)
{
	Vector2 temp = inputHandler->getLeftAnalogAxis(0);
	if (m_buttonResetTimer > REQUIRED_BUTTON_WAIT_TIME)
	{
		// MOVE LEFT ANALOGUE STICK RIGHT
		if (temp.y > 0.5)
		{
			HierarchyMenu::Instance()->ChangeSelectedButton(1);
			m_buttonResetTimer = 0;
		}
		// MOVE LEFT ANALOGUE STICK LEFT
		else if (temp.y < -0.5)
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
				switch (numReturned->y)
				{
				case 0:
					requestScenePush(SceneID::Game);
					break;
				case 1:
					Entity::resetGlobalID();
					requestScenePush(SceneID::Multiplayer);
					break;
				}
				break;
			case 1:
				requestSceneClear();
				break;
			}
		}
	}
	else if (inputHandler->isButtonPressed(JoyStickID::One, JoyStickButtonID::Button_B))
	{
		HierarchyMenu::Instance()->ReturnToPrimaryList();
	}
}

void TitleScene::update(float deltaTime)
{
	m_buttonResetTimer += deltaTime;


}

void TitleScene::render(SDL_Renderer* renderer)
{
	HierarchyMenu::Instance()->Draw(renderer);
	Entity::resetGlobalID();
}

void TitleScene::onEnter()
{
	AudioManager::Instance()->playMusic(MusicID::Theme, -1);
}

void TitleScene::onExit()
{
	AudioManager::Instance()->stopMusic();
}