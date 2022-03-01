#pragma once
#include "Client.h"
#include "Room.h"

class RoomManager
{
	friend class Room;
public:
	void RegisterRoom(Client * owner, string roomName, int maxRoomCount);
	void EnterRoom(Client* client, int roomNumber);
	void ExitRoom(Client* clint);
	void DestroyRoom(int roomNumber,Session* session);
	Room* GetRoomWithNumber(int roomNumber);
	bool CheckRoomExistByNumber(int roomNumber);
	void ShowRoomList(Session* session);
	void BroadCastToRoom(Room* room, string& msg);
	void BroadCastToRoomWithNumber(int roomNumber, string& msg);
private:
	vector<Room*> RoomList;
};

extern RoomManager GRoomManager;
