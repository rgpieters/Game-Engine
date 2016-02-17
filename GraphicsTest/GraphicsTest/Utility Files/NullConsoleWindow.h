#ifndef _NULL_CONSOLE_WINDOW_H_
#define _NULL_CONSOLE_WINDOW_H_

#include "ConsoleWindow.h"

class NullConsoleWindow : public ConsoleWindow
{
public:
	virtual void WriteToConsole(string* input)	{};
	virtual void WriteToConsole(float input)	{};
	virtual void WriteToConsole(bool input)		{};
	virtual void WriteToConsole(char* input)	{};
	virtual void WriteToConsole(int input)		{};
	virtual void WriteToConsole(string input)	{};
};

#endif