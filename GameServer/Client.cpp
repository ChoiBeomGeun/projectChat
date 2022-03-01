#include "Client.h"

Client::Client(string name, ::Session* session) : Name(name) , Session(session)
{
}

void Client::SetRoomState(int roomnumber)
{
	EntertedRoomNumber = roomnumber;
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
