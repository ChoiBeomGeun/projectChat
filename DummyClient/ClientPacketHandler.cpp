#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, unsigned len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case Login:
		Handle_Login_TEST(buffer, len);
		break;
	}
}

struct LoginData
{
	int packetSize;
	int packetId; 
	int id; 
};
// temp
void ClientPacketHandler::Handle_Login_TEST(BYTE* buffer, unsigned len)
{
	BufferReader br(buffer, len);

	if (len < sizeof(LoginData))
		return;

	LoginData pkt;
	br >> pkt;

	cout << "id" << pkt.id <<"packetID" <<  pkt.packetId << "packetSize" << pkt.packetSize;
}
