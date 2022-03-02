#pragma once
#include "pch.h"
class Session
{
public:
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

	void Reset()
	{
		RecvBytes = 0;
		ZeroMemory(RecvBuffer, BUFSIZE);
	}
};
