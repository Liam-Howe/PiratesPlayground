#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_Mixer.h>
#include <iostream>
#include <map>
#include <assert.h>



class ContentLoader
{
public:
	enum SoundType
	{
		SFX,
		MUSIC
	};

	ContentLoader();
	~ContentLoader();

	bool loadSound(std::string filePath, int soundID, SoundType soundType);
	void loadBmpTexture(std::string filePath, int textureID, bool transparent, SDL_Color* colorKey, SDL_Renderer * renderer);
	void loadPngTexture(std::string filePath, int textureID, SDL_Renderer * renderer);
	void loadFont(std::string path, std::string message, int fontSize, SDL_Color color, SDL_Rect & pos, int textureID, SDL_Renderer * pRenderer);
	SDL_Texture * getTexture(int textureID);
	void cleanUp();
	std::map<int, Mix_Chunk*> & getSoundHolder();
	std::map<int, Mix_Music*> & getMusicHolder();

private:
	std::map<int, SDL_Texture*> m_textureHolder;
	std::map<int, Mix_Chunk*> m_sfxHolder;
	std::map<int, Mix_Music*> m_musicHolder;
	ContentLoader(const ContentLoader& other) = delete; // non construction-copyable
	ContentLoader& operator=(const ContentLoader&) = delete; // non copyable
	void cleanUpTexture();
	void cleanUpSFX();
	void cleanUpMusic();
};

