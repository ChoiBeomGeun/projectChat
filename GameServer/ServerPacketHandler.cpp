#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

void ServerPacketHandler::HandlePacket(BYTE* buffer, unsigned len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	switch (header.id)
	{
	default:
		break;
	}
}
// temp
SendBuffer ServerPacketHandler::Make_Login_TEST(unsigned id)
{
	SendBuffer * sendBuffer = new SendBuffer(4096);

	BufferWriter bw(sendBuffer->Buffer(), 4096);

	PacketHeader* header = bw.Reserve<PacketHeader>();
	
	bw << id;

	header->size = bw.WriteSize();
	header->id = Login;

	return *sendBuffer;
}
