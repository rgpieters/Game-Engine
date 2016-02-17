#ifndef _NULL_SOUND_MANAGER_H_
#define _NULL_SOUND_MANAGER_H_

#include "../Manager Files/SoundManager.h"

class NullSoundManager : public SoundManager
{
	virtual void Initialize()	{};
	virtual void Shutdown()		{};
	virtual void Update()		{};
 
	virtual void LoadSoundFile(char* fileLocation, char* soundName)	{};
	virtual void PlaySoundFile(char* soundName, CHANNELS channel)		{};
	
	virtual void SetMasterVolume(float volume)			{};
	virtual void SetBackgroundMusicVolume(float volume)	{};
	virtual void SetSoundEffectsVolue(float volume)		{};
	virtual void SetPanLevel(float pan)					{};
};

#endif