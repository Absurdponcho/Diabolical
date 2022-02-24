#include "Socket.h"
#include "Logging/Logging.h"

#ifdef PLATFORM_WINDOWS
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

int DSocket::Close()
{
	LOG("Socket closed");
	int Status = 0;
#ifdef PLATFORM_WINDOWS
	Status = shutdown(Socket, SD_BOTH);
	if (Status == 0) { Status = closesocket(Socket); }
#endif
	Socket = 0;
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
	if (Server.InAddress.Address == INADDR_NONE)
	{
		struct hostent* remoteHost = gethostbyname(*Address);
		if (!remoteHost) 
		{
			LOG_ERR(DString::Format("Could not resolve remote host (%s)", *Address));
			return false;
		}
		if (remoteHost->h_addrtype != AF_INET)
		{
			LOG_ERR(DString::Format("Tried to connect to remote host (%s) which is non-IPv4", *Address));
			return false;
		}
		Server.InAddress.Address = *(uint32_t*) remoteHost->h_addr_list[0];
	}
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
	if (SendVal != SOCKET_ERROR) return true;
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

void DSocket::SetRecieveTimeoutMillis(uint32_t Millis)
{
#ifdef PLATFORM_WINDOWS
	setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&Millis, sizeof(int));
#endif
}

void DSocket::SetSendTimeoutMillis(uint32_t Millis)
{
#ifdef PLATFORM_WINDOWS
	setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&Millis, sizeof(int));
#endif
}

void DSocket::NetworkTest()
{
	DSocket Socket;
	Socket.CreateTCP();
	Socket.SetRecieveTimeoutMillis(1000);
	if (Socket.Connect("www.google.com", 80))
	{
		LOG("Successfully connected to www.google.com");
	}
	else
	{
		LOG("Failed to connect to www.google.com");
		return;
	}
	const char* Message = "GET / HTTP/1.1\r\n\r\n";
	Socket.Send(Message, (int)strlen(Message));
	char Response[4096];
	int ReceivedBytes = 1;

	while (ReceivedBytes > 0)
	{
		if (Socket.Receive(Response, 4096, ReceivedBytes))
		{
			LOG(DString::Format("Recieved response of size %i", ReceivedBytes));
		}
	}

	Socket.Close();
}

bool DSocket::StartListening()
{
#ifdef PLATFORM_WINDOWS
	if (listen(Socket, SOMAXCONN) == 0)
	{
		return true;
	}
	else 
	{
		return false;
	}
#endif

	return false;
}

bool DSocket::Bind(DString LocalAddress, int Port)
{
#ifdef PLATFORM_WINDOWS
	SocketAddress Server;
	Server.InAddress.Address = inet_addr(*LocalAddress);
	if (Server.InAddress.Address == INADDR_NONE)
	{
		LOG_ERR(DString::Format("Could not bind to local address %s", *LocalAddress));
		return false;
	}
	Server.Family = AF_INET;
	Server.Port = htons(Port);

	if (bind(Socket, (struct sockaddr *)&Server, sizeof(Server)) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
#endif

	return false;
}

bool DSocket::AcceptConnection(std::unique_ptr<DSocket>& NewSocket, DString* IncomingAddress)
{
#ifdef PLATFORM_WINDOWS
	SocketAddress SAddress;
	int AddrSize = sizeof(SAddress);
	uint64_t NewConnection = accept(Socket, (struct sockaddr *)&SAddress, &AddrSize);

	if (NewConnection == INVALID_SOCKET)
	{
		return false;
	}

	NewSocket = std::make_unique<DSocket>(NewConnection);

	if (IncomingAddress)
	{
		char *ip = inet_ntoa(*(in_addr*)&SAddress.InAddress);
		if (ip)
		{
			*IncomingAddress = DString(ip);
		}
	}

	return true;
#endif

	return false;
}
