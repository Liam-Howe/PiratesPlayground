#include "stdafx.h"
#include "Button.h"



Button::Button(SDL_Rect pos, SDL_Texture* tex, SDL_Rect sourcePos, int id)
{
	m_postion = pos;
	m_texture = tex;
	m_sourcePos = sourcePos;
	m_id = id;
}

Button::Button(SDL_Rect pos, SDL_Texture * tex, SDL_Rect sourcePos, int id, int index)
{
	m_postion = pos;
	m_texture = tex;
	m_sourcePos = sourcePos;
	m_id = id;
	m_index = index;
}


Button::~Button()
{
}

SDL_Rect Button::getPosition()
{
	return m_postion;
}

void Button::setPosition(SDL_Rect pos)
{
	m_postion = pos;
}

SDL_Texture * Button::getTexture()
{
	return m_texture;
}

void Button::setTexture(SDL_Texture* tex)
{
	m_texture = tex;
}

SDL_Rect Button::getSourcePosition()
{
	return m_sourcePos;
}

void Button::setSourcePosition(SDL_Rect pos)
{
	m_sourcePos = pos;
}

int Button::getID()
{
	return m_id;
}
int Button::getIndex()
{
	return m_index;
}
