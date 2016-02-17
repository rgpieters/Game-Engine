#include "SoundManager.h"

SoundManager::SoundManager()
{
	m_System = nullptr;
	m_MasterVolume = 1.0f;
	m_BackgroundMusicVolume = 1.0f;
	m_SoundEffectsVolume = 1.0f;
}

SoundManager::SoundManager(const SoundManager& other)
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Initialize()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		// TODO: Insert Message box here
		// FMOD_ErrorString(result)
	}

	result = m_System->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		// TODO: Insert Message box here
		// FMOD_ErrorString(result)
	}

	FMOD::Channel* tempChannel = nullptr;
	FMOD::Channel* tempChannel2 = nullptr;

	m_ChannelList.push_back(tempChannel);
	m_ChannelList.push_back(tempChannel2);
}

void SoundManager::Shutdown()
{
	for (std::map<char*, FMOD::Sound*>::iterator iter = m_SoundList.begin(); iter != m_SoundList.end(); iter++)
	{
		iter->second->release();
	}

	// Nothing needed to shutdown the channel list?

	if (m_System)
	{
		m_System->release();
		m_System = nullptr;
	}
}

void SoundManager::Update()
{
	m_System->update();
}

void SoundManager::LoadSoundFile(char* fileLocation, char* soundName, FMOD_MODE mode)
{
	FMOD_RESULT result;
	FMOD::Sound* tempSound = nullptr;

	result = m_System->createSound(fileLocation, mode, 0, &tempSound);
	if (result != FMOD_OK)
	{
		//MessageBox(0, errorInfo., L"Sound Creation Error", MB_OK);
	}
	m_SoundList.insert(std::pair<char*, FMOD::Sound*>(soundName, tempSound));
}

void SoundManager::PlaySoundFile(char* soundName, CHANNELS channel)
{
	FMOD_RESULT result;
	result = m_System->playSound(m_SoundList[soundName], 0, false, &m_ChannelList[channel]);
	if (result != FMOD_OK)
	{
		// TODO: Insert Message box here
		// FMOD_ErrorString(result)
	}
}

void SoundManager::SetMasterVolume(float volume)
{
	m_MasterVolume = volume;
	m_ChannelList[BACKGROUND_MUSIC]->setVolume(m_BackgroundMusicVolume * volume);
	m_ChannelList[SOUND_EFFECTS]->setVolume(m_SoundEffectsVolume * volume);
}

void SoundManager::SetBackgroundMusicVolume(float volume)
{
	m_BackgroundMusicVolume = volume;
	m_ChannelList[BACKGROUND_MUSIC]->setVolume(volume * m_MasterVolume);
}

void SoundManager::SetSoundEffectsVolue(float volume)
{
	m_SoundEffectsVolume = volume;
	m_ChannelList[SOUND_EFFECTS]->setVolume(volume * m_MasterVolume);
}

void SoundManager::SetPanLevel(float pan)
{
	for (unsigned int i = 0; i < m_ChannelList.size(); i++)
	{
		m_ChannelList[i]->setPan(pan);
	}
}