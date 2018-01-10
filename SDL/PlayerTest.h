#pragma once
#include "SDL.h"
#include "InputHandler.h"
class PlayerTest
{
public:
	PlayerTest(SDL_Texture * texture);
	~PlayerTest();

	void input(InputHandler * input);
	void update(float deltaTime);
	void render(SDL_Renderer * renderer);

	Vector2 getPosition();
	void setID(int ID);
	int getID();
	void setPosition(float x, float y);
private:
	int m_id;
	Vector2 m_position;
	Vector2 m_velocity;
	SDL_Point m_size;
	SDL_Texture * m_texture;
};

