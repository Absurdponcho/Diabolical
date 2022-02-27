#pragma once
#include "Thread/Thread.h"
#include "Thread/GameThread.h"
#include "Socket.h"
#include "NetTypes.h"
#include "Thread/ThreadsafeContainer.h"
#include "Types/DMemory.h"

class DTCPConnection
{
public:
	DTCPConnection(DUniquePtr<DSocket>& NewSocket, DString IncomingIP);
	virtual ~DTCPConnection();

	const DString& GetIP() const { return RemoteIP; } 
	void QueueTCPSendBuffer(DUniquePtr<NetBuffer>& Buffer);

	void SendPing();

protected:

	// Using a unique_ptr to heap allocated buffers. saves us from doing HUGE buffer copies. Thread will have full ownership.
	// during mutex lock, only ptr needs to be swapped so its much better than copying buffers
	DThreadsafeContainer<DVector<DUniquePtr<NetBuffer>>> TCPSendBuffers;

	bool PopTCPSendBuffer(DUniquePtr<NetBuffer>& Buffer);

	bool bMustClose = false;
	void TCPSend();
	void TCPReceive();

	DString RemoteIP = "";

	DUniquePtr<DSocket> TCPSocket;
	DUniquePtr<DThread> TCPSendThread;
	DUniquePtr<DThread> TCPReceiveThread;
};