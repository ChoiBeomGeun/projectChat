#include "ClientManager.h"
#include "MainServer.h"
#include "RoomManager.h"
#include "SessionManager.h"
#include "StringTable.h"

#pragma comment(lib, "ws2_32.lib")

SessionManager GSessionManager;
ClientManager GClientManager;
RoomManager GRoomManager;
int main()
{

	MainServer mainServer;
	mainServer.Start(7777);
	mainServer.Update();

}