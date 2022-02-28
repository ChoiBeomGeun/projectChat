#include "MainServer.h"
#include "SessionManager.h"

#pragma comment(lib, "ws2_32.lib")

SessionManager GSessionManager;

int main()
{
	MainServer mainServer;
	mainServer.Start(7777);
	mainServer.Update();

}