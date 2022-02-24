#pragma once
#include "Types/DString.h"

class DServer;
class DClient;

class DNetworkManager
{
public:
	static void Initialize();

	static DServer* GetServer();
	static DClient* GetClient();

	static DServer* StartServer();
	static DClient* StartClient();
protected:
	static DServer* Server;
	static DClient* Client; 

	static void TryListen(const DString& ListenString);
	static void TryJoin(const DString& JoinString);
};

