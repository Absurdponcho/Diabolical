#pragma once
#include "../Thread/Thread.h"
#include "../Thread/GameThread.h"
#include "Socket.h"
#include "TCPConnection.h"

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
	std::vector<std::unique_ptr<class DTCPConnection>> ClientConnections;

	void TCPRun();

	bool bMustClose = false;
	bool bIsBound = false;

	bool bPendingBind = false;
	DString PendingAddress = "";
	int PendingPort = 0;

	std::unique_ptr<DSocket> TCPSocket;
	std::unique_ptr<DThread> TCPThread;
};
