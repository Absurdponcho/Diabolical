#pragma once
#include "Thread/Thread.h"
#include "Thread/GameThread.h"
#include "Socket.h"
#include "TCPConnection.h"
#include "Types/DVector.h"
#include "Types/DMemory.h"

class DSocket;

class DServer
{
public:
	DServer();
	~DServer();

	void AsyncListen(const DString& LocalAddress, int Port);

	// Callback on non-game thread
	DAction<DTCPConnection*> OnConnection; // DSocket = new socket, DString = incoming IP

protected:
	DVector<DUniquePtr<class DTCPConnection>> ClientConnections;

	void TCPRun();

	bool bMustClose = false;
	bool bIsBound = false;

	bool bPendingBind = false;
	DString PendingAddress = "";
	int PendingPort = 0;

	DUniquePtr<DSocket> TCPSocket;
	DUniquePtr<DThread> TCPThread;
};
