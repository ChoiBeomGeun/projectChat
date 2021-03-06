#include "Client.h"
#include "Session.h"

Client::Client(string name, ::Session* session) : Session(session) , Name(name)
{
	
}

void Client::SetRoomState(int roomNumber)
{
	EntertedRoomNumber = roomNumber;
}

Session* Client::GetSession()
{
	return Session;
}

string Client::GetName()
{
	return Name;
}

int Client::GetEntertedRoomNumber()
{
	return EntertedRoomNumber;
}
