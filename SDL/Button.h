#pragma once
#include <SDL.h>
#include <vector>

class Button
{
public:
	Button(SDL_Rect pos, SDL_Texture* tex, SDL_Rect sourcePos, int id);
	Button(SDL_Rect pos, SDL_Texture* tex, SDL_Rect sourcePos, int id, int index);
	~Button();

	SDL_Rect getPosition();
	void setPosition(SDL_Rect pos);

	SDL_Texture* getTexture();
	void setTexture(SDL_Texture* tex);

	SDL_Rect getSourcePosition();
	void setSourcePosition(SDL_Rect pos);

	int getID();
	int getIndex();


private:
	//	Primary Button
	SDL_Rect m_postion;
	SDL_Texture* m_texture;
	SDL_Rect m_sourcePos;
	int m_id;
	int m_index;
};

