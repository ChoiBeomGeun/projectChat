#include "Room.h"

Room::Room(Client owner, string roomName, int maxRoomCount) : Owner(owner), RoomName(roomName) , MaxRoomCount(maxRoomCount)
{

}
vector<Client>& Room::GetClients()
{
	return RoomClients;
}

int Room::GetMaxRoomCount()
{
	return MaxRoomCount;
}

int Room::GetCurUserCount()
{
	return static_cast<int>(RoomClients.size());
}

string Room::GetRoomName()
{
	return RoomName;
}

Client Room::GetOwner()
{
	return Owner;
}
