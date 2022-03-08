#include "ClientManager.h"
#include "MainServer.h"
#include "RoomManager.h"
#include "SessionManager.h"
#include "StringTable.h"
#include  "PacketSet.h"
#pragma comment(lib, "ws2_32.lib")

SessionManager GSessionManager;
ClientManager GClientManager;
RoomManager GRoomManager;



struct PKT_S_TEST
{


	int packetSize; // 공용 헤더
	int packetId; // 공용 헤더
	int buffsOffset;
	int buffsCount;
	const int msgCount;
	const char* msg;
};


int main(int argc, char* argv[])
{

	int port = 7777;
	if (argc != 2) 
	{
		cout << "포트설정이 잘못되었습니다."<<endl;
		cout << "기본 포트로 연결하겠습니다." << port << endl;
	}
	else
	{
		string parse(argv[1]);
		Utility::TryParseIntFromString(port, parse);
	}

	MainServer mainServer;
	mainServer.Start(port);
	mainServer.Update();

	return 0;
}