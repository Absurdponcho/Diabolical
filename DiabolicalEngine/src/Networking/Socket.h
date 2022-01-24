#pragma once
#include "../DiabolicalEngine.h"
#include "../DiabolicalTypes.h"

union SocketAddressUnion
{
	struct
	{
		UINT8 b1, b2, b3, b4;
	} UnionBytes;
	struct
	{
		UINT16 w1, w2;
	} UnionShorts;
	UINT32 Address;
};

struct SocketAddress
{
	short Family;
	unsigned short Port;
	SocketAddressUnion InAddress;
	char Zero[8];
};

class DSocket
{
public:
	static void NetworkTest();
	static void InitSockets();
	bool CreateTCP();
	bool CreateUDP();
	bool Connect(DString Address, int Port);
	int Close();
	bool Send(const char* Buffer, int Count);
	bool Receive(char* Buffer, int BufferLength, int& RecievedBytes);

private:
#ifdef PLATFORM_WINDOWS
	UINT64 Socket = 0;
#endif
};

