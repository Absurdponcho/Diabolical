#pragma once
#include "../DiabolicalEngine.h"
#include "../DiabolicalTypes.h"

union SocketAddressUnion
{
	struct
	{
		uint8_t b1, b2, b3, b4;
	} UnionBytes;
	struct
	{
		uint16_t w1, w2;
	} UnionShorts;
	uint32_t Address;
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

	DSocket(){};
#ifdef PLATFORM_WINDOWS
	DSocket(uint64_t EstablishedConnection) : Socket(EstablishedConnection) {}
#endif

	void SetRecieveTimeoutMillis(uint32_t Millis);
	void SetSendTimeoutMillis(uint32_t Millis);
	bool CreateTCP();
	bool CreateUDP();
	bool Connect(DString Address, int Port);
	int Close();
	bool Send(const char* Buffer, int Count);
	bool Receive(char* Buffer, int BufferLength, int& RecievedBytes);
	bool StartListening();
	bool Bind(DString LocalAddress, int Port);
	bool AcceptConnection(DSocket& NewSocket);

private:


#ifdef PLATFORM_WINDOWS
	uint64_t Socket = 0;
#endif
};

