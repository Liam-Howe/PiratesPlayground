#include "SceneManager.h"
#include "HierarchyMenu.h"

class GameOverScene : public Scene
{
public:
	GameOverScene(SceneManager& sceneManager, ContentLoader& contentLoader, ThreadPool & threadPool);
	~GameOverScene();


	void handleEvents(InputHandler * inputHandler, float dt);
	void update(float deltaTime);
	void render(SDL_Renderer * renderer);

	void onEnter();
	void onExit();

private:
	SDL_Texture* m_texture;
	float m_buttonResetTimer;
	const float REQUIRED_BUTTON_WAIT_TIME = 0.2;
};