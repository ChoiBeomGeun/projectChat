#include "Room.h"

Room::Room(Client owner, string roomName, int maxRoomCount) : Owner(owner), RoomName(roomName) , MaxRoomCount(maxRoomCount)
{
	RoomClients = new vector<Client>();
}

Room::~Room()
{
	delete(RoomClients);
}

void Room::AddClient(Client* client)
{
	RoomClients->push_back(*client);
}

void Room::RemoveClient(Client* client)
{
	RoomClients->erase(
		std::remove_if(RoomClients->begin(), RoomClients->end(),
			[=](Client pClient) {return pClient.GetName() == client->GetName(); }),
		RoomClients->end());
}

vector<Client> Room::GetClients()
{
	return *RoomClients;
}

int Room::GetMaxRoomCount()
{
	return MaxRoomCount;
}

int Room::GetCurUserCount()
{
	return static_cast<int>((*RoomClients).size());
}

string Room::GetRoomName()
{
	return RoomName;
}

Client Room::GetOwner()
{
	return Owner;
}
