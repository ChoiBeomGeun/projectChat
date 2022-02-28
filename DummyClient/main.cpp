#include "pch.h"
#include <iostream>

#include "BufferWriter.h"
#include "SendBuffer.h"

#pragma comment(lib, "ws2_32.lib")


char sendBuffer[100] = "Hello Worlddddddddd";
void Send(SOCKET clientSocket)
{
	SendBuffer* sendBuffer = new SendBuffer(4096);

	BufferWriter bw(sendBuffer->Buffer(), 4096);

	PacketHeader* header = bw.Reserve<PacketHeader>();
	header->id = 42;
	header->size = 40;

	unsigned id = 500;
	unsigned hp = 100;
	unsigned attack = 1000;

	bw << id << hp << attack;




	auto test = reinterpret_cast<char*> (sendBuffer->Buffer());
	if (::send(clientSocket, reinterpret_cast<char*> (sendBuffer->Buffer()), sizeof(sendBuffer), 0) == SOCKET_ERROR)
	{
		cout << "error : " << GetLastError();
	}


}



int main()
{

}