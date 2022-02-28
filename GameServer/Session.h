#pragma once
#include "MainServer.h"

struct Session
{
	enum
	{
		BUFSIZE = 1000
	};

	enum State
	{
		PreLogin, Login, Lobby
	};

	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = "";
	int recvBytes = 0;
	int sendBytes = 0;

	void Reset()
	{
		recvBytes = 0;
		ZeroMemory(recvBuffer, BUFSIZE + 1);
	}
};
