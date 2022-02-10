#include "Server.h"
#include "Socket.h"
#include "../Check.h"

DServer::DServer()
	:	TCPSocket(std::make_unique<DSocket>()),
		TCPThread(std::make_unique<DThread>([=]() { TCPRun(); }))
{
	TCPSocket->CreateTCP();
}

DServer::~DServer()
{
	bMustClose = true;
	TCPThread->Join();
	TCPSocket->Close();
}

void DServer::AsyncListen(const DString& LocalAddress, int Port)
{
	PendingAddress = LocalAddress;
	PendingPort = Port;
	bPendingBind = true;
}

void DServer::TCPRun()
{
	while (!bMustClose)
	{
		if (!bIsBound)
		{
			if (bPendingBind)
			{
				bool bBound = TCPSocket->Bind(PendingAddress, PendingPort);
				if (!bBound)
				{
					LOG_ERR(DString::Format("Could not bind to %s:%i", *PendingAddress, PendingPort));
					bPendingBind = false;
					continue;
				}
				bool bListen = TCPSocket->StartListening();
				if (!bListen)
				{
					LOG_ERR(DString::Format("Could not listen on %s:%i", *PendingAddress, PendingPort));
					bPendingBind = false;
					continue;
				}
				bIsBound = true;

				bPendingBind = false;
			}

			if (!bIsBound)
			{
				DThread::Sleep(100);
			}
		}
		else
		{
			DSocket NewConnection;
			if (TCPSocket->AcceptConnection(NewConnection))
			{
				LOG(DString::Format("Connection Accepted"));
			}
		}
	}
}