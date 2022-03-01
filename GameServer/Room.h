#pragma once
#include "Client.h"

class Room
{
public:
	Room(Client owner,string roomName, int maxRoomCount);
	vector<Client> & GetClients();
	int GetMaxRoomCount();
	int GetCurUserCount();
	string GetRoomName();
	Client GetOwner();

private:
	Client Owner;
	string RoomName;
	int MaxRoomCount;

	vector<Client> RoomClients;
};

