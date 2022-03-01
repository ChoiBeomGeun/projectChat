#pragma once
#include "MainServer.h"

struct Session
{
	enum State
	{
	   Lobby, Room
	};

	enum
	{
		BUFSIZE = 1000
	};



	SOCKET Socket = INVALID_SOCKET;
	string Key = "";
	char RecvBuffer[BUFSIZE] = "";
	int RecvBytes = 0;
	int SendBytes = 0;
	bool IsSending = false;

	void Reset()
	{
		RecvBytes = 0;
		ZeroMemory(RecvBuffer, BUFSIZE);
	}
};
