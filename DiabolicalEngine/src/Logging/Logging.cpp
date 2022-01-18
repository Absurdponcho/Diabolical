#include "Logging.h"
#include <iostream>
#include <assert.h>
#include "../DiabolicalEngine.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

uint8_t LogColor = 15;

void SetLogColor(uint8_t Color)
{
	LogColor = Color;
#ifdef PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, Color);
#else
	assert(false);
#endif
}


void Logging::LogString(std::string Prefix, std::string Func, int Line, std::string String, int Color)
{
	SetLogColor(Color);
	std::cout << Prefix << ": " << Func << "(" << Line << "): " << String << std::endl;
	SetLogColor(15);

}
