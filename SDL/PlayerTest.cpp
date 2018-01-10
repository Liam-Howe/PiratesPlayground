#include "stdafx.h"
#include "PlayerTest.h"
#include "Vector2.h"

PlayerTest::PlayerTest( SDL_Texture * texture)
	: m_texture(texture)
	, m_position(Vector2{ 100,100 })
{
}
PlayerTest::~PlayerTest()
{
}

void PlayerTest::input(InputHandler * input)
{
	m_velocity = input->getLeftAnalogAxis(0);
}

void PlayerTest::update(float deltaTime)
{
	m_position.x += m_velocity.x * deltaTime * 100;
	m_position.y += m_velocity.y * deltaTime * 100;
}
void PlayerTest::render(SDL_Renderer * renderer)
{
	SDL_Rect dest{ m_position.x, m_position.y, 100,100 };
	SDL_RenderCopy(renderer, m_texture, NULL, &dest);
}

Vector2 PlayerTest::getPosition()
{
	return m_position;
}

void PlayerTest::setID(int ID)
{
	m_id = ID;
}

int PlayerTest::getID()
{
	return m_id;
}

void PlayerTest::setPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}
