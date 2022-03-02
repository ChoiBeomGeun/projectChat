#pragma once
#include  "pch.h"
class Client;
class Session;
class Room;
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
	void ShowRoomUserList(Session* session,int roomNumber);
	void BroadCastToRoom(Room* room, string& msg);
	void BroadCastToRoomWithNumber(int roomNumber, string& msg);
private:
	vector<Room*> RoomList;
};

extern RoomManager GRoomManager;
