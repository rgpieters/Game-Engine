#include "systemclass.h"
#include "State Files\EntryState.h"
#include "State Files\MenuState.h"
#include "State Files\OptionsState.h"
#include "State Files\GameplayState.h"


SystemClass::SystemClass()
{
	m_Graphics = nullptr;
	m_Timer = nullptr;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	ConsoleWindow* tempConsoleWindow = new ConsoleWindow();
	tempConsoleWindow->CreateConsoleWindow();

	SoundManager* tempSoundManager = new SoundManager();
	tempSoundManager->Initialize();

	Locator::Initialize();
	Locator::SetConsoleWindow(tempConsoleWindow);
	Locator::SetSoundManager(tempSoundManager);

	InitializeWindows(screenWidth, screenHeight);

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Timer = new Timer();
	m_Timer->Start();

	EntryState* tempBaseState = new EntryState();
	tempBaseState->Initialize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), m_hwnd, screenWidth, screenHeight);
	m_StateList.push_back((BaseState*)tempBaseState);

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	m_StateList[0]->Shutdown();
	Locator::Shutdown();
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));

	m_done = false;
	while (!m_done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			m_done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				m_done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	float fDt = m_Timer->GetElapsedTime();

	XMMATRIX tempView;
	switch (m_StateList.back()->Update(fDt))
	{
		case BaseState::FULL_SHUTDOWN:
		{
			m_done = true;
		}
		break;
		case BaseState::MOVE_TO_MENU:
		{
 			BaseState::STATES state = m_StateList.back()->GetState();
			
			vector<BaseState*>::iterator iter = m_StateList.end();
			m_StateList.back()->Shutdown();
			delete m_StateList.back();
			m_StateList.pop_back();

			if (state == BaseState::ENTRY_STATE)
			{
				MenuState* tempBaseState = new MenuState();
				tempBaseState->Initialize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), m_hwnd, m_screenWidth, m_screenHeight);
				m_StateList.push_back((BaseState*)tempBaseState);
			}
		}
		break;
		case BaseState::MOVE_TO_GAMEPLAY:
		{
			BaseState::STATES state = m_StateList.back()->GetState();

			m_StateList.back()->Shutdown();
			delete m_StateList.back();
			m_StateList.pop_back();

			if (state == BaseState::MENU_STATE)
			{
				GameplayState* tempBaseState = new GameplayState();
				tempBaseState->Initialize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), m_hwnd, m_screenWidth, m_screenHeight);
				m_StateList.push_back((BaseState*)tempBaseState);
			}
		}
		break;
		case BaseState::MOVE_TO_OPTIONS:
		{
			OptionsState* tempBaseState = new OptionsState();
			tempBaseState->Initialize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), m_hwnd, m_screenWidth, m_screenHeight);
			m_StateList.push_back((BaseState*)tempBaseState);
		}
		break;
	}

	m_Graphics->RenderStart();
	m_StateList.back()->Render(m_Graphics->GetDeviceContext(), tempView, m_Graphics->GetOrthoMatrix());

	m_Graphics->RenderEnd();

	Locator::Update(fDt);
	m_Timer->Update();

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (m_StateList.size() != 0)
		return m_StateList.back()->MessageHandler(hwnd, umsg, wparam, lparam);
	else
		return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = 50;// (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = 50;// (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}