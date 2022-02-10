#include "NetworkManager.h"
#include "../CommandLine/CmdLine.h"
#include "../Logging/Logging.h"
#include "../Check.h"
#include "Server.h"
#include "Client.h"

DServer* DNetworkManager::Server = nullptr;
DClient* DNetworkManager::Client = nullptr;

void DNetworkManager::Initialize()
{
	if (const DString* HostPort = DCommandLine::GetValue("-host"))
	{
		TryListen(*HostPort);
	}

	if (const DString* JoinVal = DCommandLine::GetValue("-join"))
	{
		TryJoin(*JoinVal);
	}
}

void DNetworkManager::TryListen(const DString& ListenString)
{
	DString IP;
	DString PortStr;
	bool bSplit = ListenString.Split(":", IP, PortStr);
	if (!bSplit)
	{
		LOG_ERR(DString::Format("Invalid listen string %s", ListenString));
		return;
	}
	Check(bSplit);

	int Port = 0;
	bool bGetPort = PortStr.TryParseInt(Port);
	if (!bGetPort)
	{
		LOG_ERR(DString::Format("Invalid listen port %s", PortStr));
		return;
	}
	Check(bGetPort);

	LOG(DString::Format("Hosting %s", *ListenString));
	if (DServer* NewServer = StartServer())
	{
		NewServer->OnConnection = DAction<DTCPConnection*>([](DTCPConnection* Connection)
		{
			Check(Connection);
			LOG(DString::Format("Connection recieved from %s", *(Connection->GetIP())));

			std::unique_ptr<NetBuffer> Buffer = std::make_unique<NetBuffer>(128);
			const char* TestMessage = "Test Network Message";
			strcpy_s(Buffer->Buffer, 21, TestMessage);
			Connection->QueueTCPSendBuffer(Buffer);
		});
		NewServer->AsyncListen(IP, Port);
	}

}

void DNetworkManager::TryJoin(const DString& JoinString)
{
	DString IP;
	DString PortStr;
	bool bSplit = JoinString.Split(":", IP, PortStr);
	if (!bSplit)
	{
		LOG_ERR(DString::Format("Invalid join string %s", JoinString));
		return;
	}
	Check(bSplit);	

	int Port = 0;
	bool bGetPort = PortStr.TryParseInt(Port);
	if (!bGetPort)
	{
		LOG_ERR(DString::Format("Invalid join port %s", PortStr));
		return;
	}
	Check(bGetPort);	

	LOG(DString::Format("Joining %s", *JoinString));
	if (DClient* NewClient = StartClient())
	{
		NewClient->OnConnect = DAction<bool>([](bool bSuccess)
		{
			if (bSuccess)
			{
				LOG("Connection Success");
			}
			else
			{
				LOG_WARN("Failed to connect");
			}
		});
		NewClient->AsyncConnect(IP, Port);
	}
	
}

DServer* DNetworkManager::GetServer()
{
	return Server;
}

DClient* DNetworkManager::GetClient()
{
	return Client;
}

DServer* DNetworkManager::StartServer()
{
	Check(!Server);
	if (Server)
	{
		LOG_ERR("There can only be one instance of a DServer active at any time!");
		return nullptr;
	}

	Server = new DServer();
	return Server;
}

DClient* DNetworkManager::StartClient()
{
	Check(!Client);
	if (Client)
	{
		LOG_ERR("There can only be one instance of a DClient active at any time!");
		return nullptr;
	}

	Client = new DClient();
	return Client;
}