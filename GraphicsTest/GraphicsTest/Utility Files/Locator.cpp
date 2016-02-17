#include "Locator.h"

void Locator::Initialize()
{ 
	m_consoleWindow = &m_nullConsoleWindow;
	m_soundManger = &m_nullSoundManager;
}
void Locator::Shutdown()
{ 
	if (m_consoleWindow)
	{
		delete m_consoleWindow;
		m_consoleWindow = NULL;
	}
	if (m_soundManger)
	{
		m_soundManger->Shutdown();
		delete m_soundManger;
		m_soundManger = NULL;
	}
}

void Locator::Update(float fDt)
{
	m_soundManger->Update();
}

ConsoleWindow& Locator::GetConsoleWindow()
{ 
	return *m_consoleWindow;
}

SoundManager& Locator::GetSoundManager()
{
	return *m_soundManger;
}

void Locator::SetConsoleWindow(ConsoleWindow* consoleWindowService)
{
	if (consoleWindowService == NULL)
	{
		m_consoleWindow = &m_nullConsoleWindow;
	}
	else
	{
		m_consoleWindow = consoleWindowService;
	}
}

void Locator::SetSoundManager(SoundManager* soundManager)
{
	if (soundManager == NULL)
	{
		m_soundManger = &m_nullSoundManager;
	}
	else
	{
		m_soundManger = soundManager;
	}
}