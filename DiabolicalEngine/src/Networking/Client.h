#pragma once
#include "Thread/Thread.h"
#include "Thread/GameThread.h"

class DSocket;

class DClient
{
public:
	DClient();
	~DClient();
	void AsyncConnect(const DString& Address, int Port);

	// Callback on non-game thread
	DAction<bool> OnConnect; // bool = success

protected:
	std::unique_ptr<class DTCPConnection> TCPConnection;
	void TCPRun();

	bool bMustClose = false;
	bool bIsConnected = false;

	bool bPendingConnect = false;
	DString PendingAddress = ""; 
	int PendingPort = 0;

	std::unique_ptr<DSocket> TCPSocket;
	std::unique_ptr<DThread> TCPThread;
};

