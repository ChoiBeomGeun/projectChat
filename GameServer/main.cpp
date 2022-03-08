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


	int packetSize; // ���� ���
	int packetId; // ���� ���
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
		cout << "��Ʈ������ �߸��Ǿ����ϴ�."<<endl;
		cout << "�⺻ ��Ʈ�� �����ϰڽ��ϴ�." << port << endl;
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