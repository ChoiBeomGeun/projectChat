#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

#include "BufferReader.h"
#include "BufferWriter.h"
#include "SendBuffer.h"
#include "ServerPacketHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

const int BUFSIZE = 4096;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = "323232323";
	int recvBytes = 0;
	int sendBytes = 0;
};
void Send(SOCKET clientSocket)
{
	

	SendBuffer sendBuffer = ServerPacketHandler::Make_Login_TEST(100);

	BufferWriter bw(sendBuffer.Buffer(), sizeof(sendBuffer.Buffer()));

	PacketHeader* header = bw.Reserve<PacketHeader>();
	header->id = 42;
	header->size = 40;

	unsigned id = 500;

	bw << id;




	auto test = reinterpret_cast<char*> (sendBuffer.Buffer());
	if (::send(clientSocket, reinterpret_cast<char*> (sendBuffer.Buffer()), sizeof(sendBuffer), 0) == SOCKET_ERROR)
	{
		cout << "error : " << GetLastError();


	}


}

void inputFunc()
{
	while (1)
	{
		const char* buffer = "";


		sscanf(buffer, "&s");

		cout << buffer;
	}
	


}

int networkFunc()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	vector<Session> sessions;
	sessions.reserve(100);



	while (true)
	{
		fd_set reads;
		fd_set writes;

		FD_ZERO(&reads);
		FD_ZERO(&writes);

		FD_SET(listenSocket, &reads);

		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		int retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			return 0;

		// Listener 
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}

		for (Session& s : sessions)
		{
			 //Read
			if (FD_ISSET(s.socket, &reads))
			{
				int recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					// TODO : sessions 제거
					continue;
				}
				
				s.recvBytes = recvLen;
				cout << "recv Lng : " << recvLen << endl;
			}

			// Write
			if (FD_ISSET(s.socket, &writes))
			{
				
				int sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR)
				{
					// TODO : sessions 제거
					continue;
				}

				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}

				cout << "send Lng : " << sendLen << endl;
			}

			const char* temp = "tetete";
			::send(s.socket, temp, sizeof(temp), 0);
		}
	}

	::WSACleanup();
}
int main()
{
	thread t1(inputFunc);
	thread t2(networkFunc);
	t1.join();
	t2.join();
}