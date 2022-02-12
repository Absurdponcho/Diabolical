#pragma once
#include "../Thread/Thread.h"
#include "../Thread/GameThread.h"
#include "Socket.h"
#include "NetTypes.h"

class DTCPConnection
{
public:
	DTCPConnection(std::unique_ptr<DSocket>& NewSocket, DString IncomingIP);
	virtual ~DTCPConnection();

	const DString& GetIP() const { return RemoteIP; } 
	void QueueTCPSendBuffer(std::unique_ptr<NetBuffer>& Buffer);

protected:

	// Using a unique_ptr to heap allocated buffers. saves us from doing HUGE buffer copies. Thread will have full ownership.
	// during mutex lock, only ptr needs to be swapped so its much better than copying buffers
	DVector<std::unique_ptr<NetBuffer>> TCPSendBuffers;

	bool PopTCPSendBuffer(std::unique_ptr<NetBuffer>& Buffer);

	bool bMustClose = false;
	void TCPSend();
	void TCPReceive();

	DString RemoteIP = "";

	DMutexHandle TCPSendMutex;
	DMutexHandle TCPReceiveMutex;

	std::unique_ptr<DSocket> TCPSocket;
	std::unique_ptr<DThread> TCPSendThread;
	std::unique_ptr<DThread> TCPReceiveThread;
};