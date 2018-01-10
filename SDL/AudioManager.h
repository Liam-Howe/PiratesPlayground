#pragma once
#include <map>
#include "SDL_mixer.h"
#include "ContentLoader.h"

class AudioManager
{
public:
	void init(ContentLoader * contentLoader);
	void playSound(int soundID, int loop);
	void playMusic(int soundID, int loop);
	void stopAllSounds();
	void stopMusic();
	void pauseMusic();
	void resumeMusic();
	static AudioManager * Instance();

private:
	bool m_pause;
	std::map<int, Mix_Chunk*> * m_sfx;
	std::map<int, Mix_Music*> * m_music;

	static AudioManager *m_inst;
	AudioManager();
	~AudioManager();

	// Stops Copying
	AudioManager(const AudioManager&);
	AudioManager &operator=(const AudioManager&);

};