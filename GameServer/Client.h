#pragma once
#include "pch.h"
#include "Session.h"

class Client
{
public:
	;

	Client(string name,Session* session);

public:
	void SetRoomState(int roomNumber);

public:
	Session * GetSession();
	string GetName();
	int GetEntertedRoomNumber();

private:
	string Name;
	Session *Session;
	//if Client is not in ChatRoom , -1
	int EntertedRoomNumber = -1;
};

