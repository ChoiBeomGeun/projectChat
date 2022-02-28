#include <WS2tcpip.h>

#include "pch.h"
#include "CorePch.h"

#pragma comment(lib, "ws2_32.lib")

#include "ThreadManager.h"
#include  "Listener.h"
#include "SendBuffer.h"
#include  "Service.h"
#include  "Session.h"
#include "GameSession.h"

int main()
{


	ServerServiceRef service = MakeShared<ServerService>(
	NetAddress(L"127.0.0.1", 7777),
	MakeShared<IocpCore>(),
	MakeShared<GameSession>, // TODO : SessionManager 등
	100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}
