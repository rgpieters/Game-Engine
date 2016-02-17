#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <Windows.h>
#include <fmod.hpp>
#include <map>
#include <vector>

class SoundManager
{
public:
	enum CHANNELS { BACKGROUND_MUSIC, SOUND_EFFECTS, NUM_CHANNELS};
	SoundManager();
	SoundManager(const SoundManager& other);
	~SoundManager();

	void Initialize();
	void Shutdown();
	void Update();

	void LoadSoundFile(char* fileLocation, char* soundName, FMOD_MODE mode = FMOD_DEFAULT);
	void PlaySoundFile(char* soundName, CHANNELS channel);

	void SetMasterVolume(float volume);
	void SetBackgroundMusicVolume(float volume);
	void SetSoundEffectsVolue(float volume);
	void SetPanLevel(float pan);
private:
	FMOD::System* m_System;
	std::map<char*, FMOD::Sound*>	m_SoundList;
	std::vector<FMOD::Channel*>		m_ChannelList;

	// Volumes all 0.0 <-> 1.0
	float m_MasterVolume; 
	float m_BackgroundMusicVolume;
	float m_SoundEffectsVolume;

	// -1.0(left) to 1.0(right), default = 0(center)
	float m_PanLevel;
};

#endif