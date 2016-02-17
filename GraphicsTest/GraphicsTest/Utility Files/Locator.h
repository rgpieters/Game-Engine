#ifndef _LOCATOR_H_
#define _LOCATOR_H_

#include "ConsoleWindow.h"
#include "NullConsoleWindow.h"
#include "../Manager Files/SoundManager.h"
#include "NullSoundManager.h"

class Locator
{
public:
	static void Initialize();
	static void Shutdown();
	static ConsoleWindow& GetConsoleWindow();
	static SoundManager& GetSoundManager();

	static void Update(float fDt);

	static void SetConsoleWindow(ConsoleWindow* consoleWindowService);
	static void SetSoundManager(SoundManager* soundManager);
};
	static ConsoleWindow*		m_consoleWindow;
	static NullConsoleWindow	m_nullConsoleWindow;
	static SoundManager*		m_soundManger;
	static NullSoundManager		m_nullSoundManager;
#endif