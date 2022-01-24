#pragma once
#include "../DiabolicalEngine.h"
#ifdef PLATFORM_WINDOWS
	#include <winsock2.h>
	#include <Ws2tcpip.h>
#endif

class DSocket
{
public:
	static void InitSockets();
	int Close();

private:
#ifdef PLATFORM_WINDOWS
	SOCKET Socket;
#endif
};

