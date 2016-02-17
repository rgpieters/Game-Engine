#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow()
{

}

void ConsoleWindow::CreateConsoleWindow()
{
	AllocConsole();

	HWND consoleWindow = GetConsoleWindow();

	SetWindowPos(consoleWindow, 0, 875, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	// TODO: this might need to be change to go the right when I'm at home
}

void ConsoleWindow::WriteToConsole(string* input)
{
	LPDWORD temp = nullptr;

	wostringstream ss;
	ss << input;
	ss << '\n';

	int error = WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
		ss.str().data(),
		ss.str().size(),
		temp,
		nullptr);
}

void ConsoleWindow::WriteToConsole(float input)
{
	LPDWORD temp = nullptr;

	wostringstream ss;
	ss << input;
	ss << '\n';

	int error = WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
		ss.str().data(),
		ss.str().size(),
		temp,
		nullptr);
}

void ConsoleWindow::WriteToConsole(bool input)
{
	LPDWORD temp = nullptr;

	wostringstream ss;
	ss << input;
	ss << '\n';

	int error = WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
		ss.str().data(),
		ss.str().size(),
		temp,
		nullptr);
}
void ConsoleWindow::WriteToConsole(char* input)
{
	LPDWORD temp = nullptr;

	wostringstream ss;
	ss << input;
	ss << '\n';

	int error = WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
		ss.str().data(),
		ss.str().size(),
		temp,
		nullptr);
}

void ConsoleWindow::WriteToConsole(int input)
{
	LPDWORD temp = nullptr;

	wostringstream ss;
	ss << input;
	ss << '\n';

	int error = WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
		ss.str().data(),
		ss.str().size(),
		temp,
		nullptr);
}