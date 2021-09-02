#pragma once
#include "EngineMain.h"

#ifdef PLATFORM_WINDOWS

	#define WIN32_LEAN_AND_MEAN

	#ifdef _MSC_VER
		#include <Windows.h>
	#endif

	#include <gl/glew.h>

#endif