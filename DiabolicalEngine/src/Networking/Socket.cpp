#include "Socket.h"
#include "../Logging/Logging.h"

#ifdef PLATFORM_WINDOWS
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

int DSocket::Close()
{
	int Status = 0;
#ifdef PLATFORM_WINDOWS
	Status = shutdown(Socket, SD_BOTH);
	if (Status == 0) { Status = closesocket(Socket); }
#endif
	return Status;
}

void DSocket::InitSockets()
{
#ifdef PLATFORM_WINDOWS
	WSADATA wsa;
	int Sock = WSAStartup(MAKEWORD(2,2),&wsa);
	Check (Sock == 0);
	if (Sock != 0)
	{
		LOG_ERR("WinSocket Not Initialized");
		abort();
	}
	LOG("WinSocket Initialized");
#endif
}
