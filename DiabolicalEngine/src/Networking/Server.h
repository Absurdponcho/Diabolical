#pragma once
#include "../Thread/Thread.h"
#include "../Thread/GameThread.h"

class DSocket;

class DServer
{
public:
	DServer();
	~DServer();

	void AsyncListen(const DString& LocalAddress, int Port);

protected:
	void TCPRun();

	bool bMustClose = false;
	bool bIsBound = false;

	bool bPendingBind = false;
	DString PendingAddress = "";
	int PendingPort = 0;

	std::unique_ptr<DSocket> TCPSocket;
	std::unique_ptr<DThread> TCPThread;
};

