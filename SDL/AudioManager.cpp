#include "stdafx.h"
#include "AudioManager.h"


AudioManager * AudioManager::m_inst = nullptr;

AudioManager::AudioManager()
	: m_pause(false)
{
}
AudioManager::~AudioManager()
{
	Mix_CloseAudio();
}

void AudioManager::init(ContentLoader * contentloader)
{
	m_sfx = &contentloader->getSoundHolder();
	m_music = &contentloader->getMusicHolder();
}

void AudioManager::playSound(int soundID, int loop)
{
	Mix_PlayChannel(-1, (*m_sfx)[soundID], loop);
}



void AudioManager::playMusic(int soundID, int loop)
{
	Mix_PlayMusic((*m_music)[soundID], loop);
}

void AudioManager::stopAllSounds()
{
	Mix_HaltChannel(-1);
}

void AudioManager::stopMusic()
{
	Mix_HaltMusic();
}

void AudioManager::pauseMusic()
{
	Mix_PauseMusic();
}

void AudioManager::resumeMusic()
{
	Mix_ResumeMusic();
}

AudioManager * AudioManager::Instance()
{
	if (!m_inst)
	{
		m_inst = new AudioManager();
	}
	return m_inst;
}
