#include "TCPConnection.h"
#include "Socket.h"
#include "Thread/Thread.h"

DTCPConnection::DTCPConnection(std::unique_ptr<DSocket>& NewSocket, DString IncomingIP)
	: RemoteIP(IncomingIP),
	TCPSendThread(std::make_unique<DThread>([=]() { TCPSend(); })),
	TCPReceiveThread(std::make_unique<DThread>([=]() { TCPReceive(); }))
{
	Check(NewSocket.get());
	TCPSocket.swap(NewSocket);
}

DTCPConnection::~DTCPConnection()
{
	bMustClose = true;
	TCPSendThread->Join();
	TCPReceiveThread->Join();
	TCPSocket->Close();
}

void DTCPConnection::TCPSend()
{
	std::unique_ptr<NetBuffer> Buffer;

	while (!bMustClose)
	{
		while (PopTCPSendBuffer(Buffer))
		{
			if (!TCPSocket->Send(Buffer->Buffer, Buffer->Size))
			{
				LOG_ERR("Failed to send bytes");
			}
		}
		DThread::Sleep(10);
	}
}

void DTCPConnection::TCPReceive()
{
	char RecvBuffer[4096];

	while (!bMustClose)
	{
		int ReceivedBytes = 0;
		while (TCPSocket->Receive(RecvBuffer, 4096, ReceivedBytes))
		{
			if (ReceivedBytes != 0)
			{
				LOG(DString::Format("Recieved bytes: %s", RecvBuffer));
			}
		}
		DThread::Sleep(10);
	}
}

void DTCPConnection::QueueTCPSendBuffer(std::unique_ptr<NetBuffer>& Buffer)
{
	auto BuffersValue = TCPSendBuffers.Retrieve();
	BuffersValue->PushBack(std::unique_ptr<NetBuffer>(Buffer.release()));
}

bool DTCPConnection::PopTCPSendBuffer(std::unique_ptr<NetBuffer>& Buffer)
{
	auto BuffersValue = TCPSendBuffers.Retrieve();
	if (BuffersValue->Size() == 0) return false;
	Buffer.swap(BuffersValue.Get()[0]);
	BuffersValue->RemoveAt(0);
	return true;
}

void DTCPConnection::SendPing()
{
	std::unique_ptr<NetBuffer> Buffer = std::make_unique<NetBuffer>(5);
	const char* TestMessage = "Ping";
	strcpy_s(Buffer->Buffer, 5, TestMessage);
	QueueTCPSendBuffer(Buffer);
}
