#ifndef _SYSTEM_CLASS_H_
#define _SYSTEM_CLASS_H_
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "Graphics Files\graphicsclass.h"
#include "Utility Files\Timer.h"
#include "Utility Files\ConsoleWindow.h"
#include "Utility Files\Locator.h"

#include "State Files\BaseState.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	bool m_done;
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	int m_screenWidth;
	int m_screenHeight;

	Timer*			m_Timer;
	GraphicsClass*	m_Graphics;
	vector<BaseState*> m_StateList;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif