#include "stdafx.h"
#include "ContentLoader.h"
#include <SDL_ttf.h>

ContentLoader::ContentLoader()
	: m_textureHolder()
	, m_sfxHolder()
	, m_musicHolder()
{
	Mix_OpenAudio(44100, AUDIO_S16, 2, 4096);
}


ContentLoader::~ContentLoader()
{
}


bool ContentLoader::loadSound(std::string filePath, int soundID, SoundType soundType)
{
	if (soundType == SFX)
	{
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
		if (chunk == nullptr)
		{
			std::cout << "Cannot Load SFX" << soundID << std::endl;
			return false;
		}
		m_sfxHolder[soundID] = chunk;
		return true;
	}
	else if (soundType == MUSIC)
	{
		Mix_Music* music = Mix_LoadMUS(filePath.c_str());
		if (music == nullptr)
		{
			std::cout << "Cannot Load Music" << std::endl;
			return false;
		}
		m_musicHolder[soundID] = music;
		return true;
	}
	return false;
}

void ContentLoader::loadBmpTexture(std::string filePath, int textureID, bool transparent, SDL_Color * colorKey, SDL_Renderer * renderer)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());

	if (transparent && colorKey != NULL)
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, colorKey->r, colorKey->g, colorKey->b));
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	m_textureHolder[textureID] = texture;
}

void ContentLoader::loadPngTexture(std::string filePath, int textureID, SDL_Renderer * renderer)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	m_textureHolder[textureID] = texture;
}

void ContentLoader::loadFont(std::string path, std::string message, int fontSize, SDL_Color color, SDL_Rect &pos, int textureID, SDL_Renderer* pRenderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	TTF_Init();
	TTF_Font* m_font = NULL;
	m_font = TTF_OpenFont(path.c_str(), fontSize);

	SDL_Surface *surf = TTF_RenderText_Blended(m_font, message.c_str(), color);

	newTexture = SDL_CreateTextureFromSurface(pRenderer, surf);

	int iW, iH;
	SDL_QueryTexture(newTexture, NULL, NULL, &iW, &iH);
	pos = SDL_Rect{ pos.x,  pos.y, iW, iH };

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);
	//Close the font that was used
	TTF_CloseFont(m_font);

	m_textureHolder[textureID] = newTexture;
}

SDL_Texture * ContentLoader::getTexture(int textureID)
{
	auto found = m_textureHolder.find(textureID);
	assert(found != m_textureHolder.end());
	return found->second;
}

void ContentLoader::cleanUp()
{
	cleanUpTexture();
	cleanUpMusic();
	cleanUpSFX();
	SDL_CloseAudio();
}

std::map<int, Mix_Chunk*> & ContentLoader::getSoundHolder()
{
	return m_sfxHolder;
}

std::map<int, Mix_Music*> & ContentLoader::getMusicHolder()
{
	return m_musicHolder;
}

void ContentLoader::cleanUpTexture()
{
	std::map<int, SDL_Texture*>::iterator itr = m_textureHolder.begin();
	while (itr != m_textureHolder.end())
	{
		SDL_DestroyTexture((*itr).second);
		(*itr).second = nullptr;
		itr++;
	}
	m_textureHolder.clear();
}

void ContentLoader::cleanUpSFX()
{
	std::map<int, Mix_Chunk*>::iterator itr = m_sfxHolder.begin();
	while (itr != m_sfxHolder.end())
	{
		Mix_FreeChunk((*itr).second);
		(*itr).second = nullptr;
		itr++;
	}
	m_sfxHolder.clear();
}

void ContentLoader::cleanUpMusic()
{
	std::map<int, Mix_Music*>::iterator itr = m_musicHolder.begin();
	while (itr != m_musicHolder.end())
	{
		Mix_FreeMusic((*itr).second);
		(*itr).second = nullptr;
		itr++;
	}
	m_musicHolder.clear();
}
