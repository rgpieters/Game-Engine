#ifndef _CONSOLE_WINDOW_H_
#define _CONSOLE_WINDOW_H_

#include <Windows.h>
#include <string>
#include <sstream>
using namespace std;

class ConsoleWindow
{
public:
	ConsoleWindow();
	void CreateConsoleWindow();

	void WriteToConsole(string* input);
	void WriteToConsole(float input);
	void WriteToConsole(bool input);
	void WriteToConsole(char* input);
	void WriteToConsole(int input);
	void WriteToConsole(string input);
};

#endif