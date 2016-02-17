#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

//#include "Messaging/IListener.h"

#include <Windows.h>
#include <strsafe.h>
#include <cstring>
#include "../Utility Files/VK_Codes.h"

class Event;

class InputManager//: public IListener
{
	enum DeviceEnum
	{
		KEYBOARD, MOUSE, GAMEPAD, JOYSTICK, NUMDEVICES
	};

private:
	RAWINPUTDEVICE  m_rawDevices[NUMDEVICES];

	void RegisterDevices();
	void HandleEvent(Event* pEvent);

public:
	InputManager();
	~InputManager();

	USHORT GetInput(LPARAM _lParam, bool& keyUp);
	void Shutdown();
};

#endif