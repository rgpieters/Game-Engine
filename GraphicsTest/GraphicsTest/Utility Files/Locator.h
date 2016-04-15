#ifndef _LOCATOR_H_
#define _LOCATOR_H_

#include "ConsoleWindow.h"
#include "NullConsoleWindow.h"
#include "../Manager Files/SoundManager.h"
#include "NullSoundManager.h"
#include "../Graphics Files/FrustumClass.h"
#include "NullFrustum.h"

class Locator
{
public:
	static void Initialize();
	static void Shutdown();
	static ConsoleWindow& GetConsoleWindow();
	static SoundManager& GetSoundManager();
	static FrustumClass& GetFrustum();

	static void Update(float fDt);

	static void SetConsoleWindow(ConsoleWindow* consoleWindowService);
	static void SetSoundManager(SoundManager* soundManager);
	static void SetFrustum(FrustumClass* frustum);
};
	static ConsoleWindow*		m_consoleWindow;
	static NullConsoleWindow	m_nullConsoleWindow;
	static SoundManager*		m_soundManger;
	static NullSoundManager		m_nullSoundManager;
	static FrustumClass*		m_frustum;
	static NullFrustum			m_nullFrustum;
#endif