#include "Client.h"
#include "Socket.h"
#include "../Check.h"

DClient::DClient()
	:	TCPSocket(std::make_unique<DSocket>()),
		TCPThread(std::make_unique<DThread>([=]() { TCPRun(); }))
{
	TCPSocket->CreateTCP();
}

DClient::~DClient()
{
	bMustClose = true;
	TCPThread->Join();
	TCPSocket->Close();
}

void DClient::AsyncConnect(const DString& Address, int Port)
{
	Check (!bIsConnected);
	Check (!bPendingConnect);
	Check(TCPSocket.get());
	Check(TCPThread.get());

	PendingAddress = Address;
	PendingPort = Port;
	bPendingConnect = true;
}

void DClient::TCPRun()
{
	while (!bMustClose)
	{
		if (!bIsConnected)
		{
			if (bPendingConnect)
			{
				if (TCPSocket->Connect(PendingAddress, PendingPort))
				{
					bPendingConnect = false;
					bIsConnected = true;
					OnConnect.Invoke(true);
				}
				else
				{
					bPendingConnect = false;
					OnConnect.Invoke(false);
				}
			}

			if (!bIsConnected)
			{
				// wait for connection
				DThread::Sleep(100);
			}
		}
		else
		{

		}
	}
}
