#pragma once
#include "pch.h"

class Client;
class Room
{

public:
	Room(Client * owner,string roomName, int maxRoomCount);
	~Room();

	void AddClient(Client* client);
	void RemoveClient(Client* client);

	vector<Client*> GetClients();
	int GetMaxRoomCount();
	int GetCurUserCount();
	string GetRoomName();
	Client GetOwner();

private:
	Client * Owner;
	string RoomName;
	int MaxRoomCount;

	vector<Client*> RoomClients;
};

