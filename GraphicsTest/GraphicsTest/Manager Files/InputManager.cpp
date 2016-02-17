#include "InputManager.h"
//#include "Facade.h"

InputManager::InputManager()
{
	RegisterDevices();
	//Facade::GetInstance()->RegisterClient("CameraMoveFoward", this);
}

InputManager::~InputManager()
{

}

USHORT InputManager::GetInput(LPARAM _lParam, bool& keyUp)
{
	UINT dwSize;
	
	GetRawInputData((HRAWINPUT)_lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return VKEY_FAILURE;
	}
	
	if (GetRawInputData((HRAWINPUT)_lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
	{
		OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
	}

	RAWINPUT* raw = (RAWINPUT*)lpb;
	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		int returnKey = raw->data.keyboard.VKey;
		UINT Event;
		Event = raw->data.keyboard.Message;

		if (Event == WM_KEYDOWN)
		{
			keyUp = false;
		}
		else if (Event == WM_KEYUP)
		{
			keyUp = true;
		}

		delete[] lpb;
		return returnKey;
	}

	delete[] lpb;
	return 0;
}

// all of these are incorrect aside from the keyboard
void InputManager::RegisterDevices()
{
	m_rawDevices[KEYBOARD].usUsagePage = 0x01;
	m_rawDevices[KEYBOARD].usUsage = 0X06;
	m_rawDevices[KEYBOARD].dwFlags = RIDEV_NOLEGACY;
	m_rawDevices[KEYBOARD].hwndTarget = 0;

	m_rawDevices[MOUSE].usUsagePage = 0x01;
	m_rawDevices[MOUSE].usUsage = 0X02;
	m_rawDevices[MOUSE].dwFlags = RIDEV_NOLEGACY;
	m_rawDevices[MOUSE].hwndTarget = 0;

	m_rawDevices[GAMEPAD].usUsagePage = 0x01;
	m_rawDevices[GAMEPAD].usUsage = 0X05;
	m_rawDevices[GAMEPAD].dwFlags = 0;
	m_rawDevices[GAMEPAD].hwndTarget = 0;

	m_rawDevices[JOYSTICK].usUsagePage = 0x01;
	m_rawDevices[JOYSTICK].usUsage = 0X04;
	m_rawDevices[JOYSTICK].dwFlags = 0;
	m_rawDevices[JOYSTICK].hwndTarget = 0;

	if (RegisterRawInputDevices(m_rawDevices, NUMDEVICES, sizeof(m_rawDevices[0])) == FALSE)
	{
		int x = 10;
		//registration failed. Call GetLastError for the cause of the error
	}
}

void InputManager::Shutdown()
{

}

void InputManager::HandleEvent(Event* pEvent)
{
	
}