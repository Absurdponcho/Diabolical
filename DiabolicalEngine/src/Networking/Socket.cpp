#include "Socket.h"
#include "../Logging/Logging.h"

#ifdef PLATFORM_WINDOWS
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <winsock2.h>
	#include <Ws2tcpip.h>
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

bool DSocket::CreateTCP()
{
	Check (!Socket);
	if (Socket) return false;

#ifdef PLATFORM_WINDOWS
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	Check (Socket != INVALID_SOCKET);
	if (Socket) return true;
#endif
	return false;
}

bool DSocket::CreateUDP()
{
	Check(!Socket);
	if (Socket) return false;

#ifdef PLATFORM_WINDOWS
	Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	Check(Socket != INVALID_SOCKET);
	if (Socket) return true;
#endif
	return false;
}

bool DSocket::Connect(DString Address, int Port)
{
#ifdef PLATFORM_WINDOWS
	SocketAddress Server;
	Server.InAddress.Address = inet_addr(*Address);
	Server.Family = AF_INET;
	Server.Port = htons(Port);

	int ConnectionStatus = connect(Socket, (struct sockaddr *)&Server, sizeof(Server));
	if (ConnectionStatus != 0)
	{
		LOG_WARN("Could not connect to remote address");
		return false;
	}

	return true;
#endif
	return false;
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

bool DSocket::Send(const char* Buffer, int Count)
{
#ifdef PLATFORM_WINDOWS
	int SendVal = send(Socket, Buffer, Count, 0);
	if (SendVal == 0) return true;
#endif
	 
	return false;
}

bool DSocket::Receive(char* Buffer, int BufferLength, int& RecievedBytes)
{
#ifdef PLATFORM_WINDOWS
	RecievedBytes = recv(Socket, Buffer, BufferLength, 0);
	if (RecievedBytes < 0 || RecievedBytes == SOCKET_ERROR) 
	{
		return false; 
	}
	return true;
#endif

	return false;
}

void DSocket::NetworkTest()
{
	DSocket Socket;
	Socket.CreateTCP();
	if (Socket.Connect("142.250.70.174", 80))
	{
		LOG("Successfully connected to your mum");
	}
	else
	{
		LOG("Failed to connect to your mum");
	}
	const char* Message = "GET / HTTP/1.1\r\n\r\n";
	Socket.Send(Message, (int)strlen(Message));
	char Response[4096];
	int ReceivedBytes = 0;
	if (Socket.Receive(Response, 4095, ReceivedBytes))
	{
		Response[ReceivedBytes] = 0;
		LOG(Response);
	}

	Socket.Close();
}
