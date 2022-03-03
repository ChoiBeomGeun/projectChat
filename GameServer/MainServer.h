#pragma once

#include "pch.h"
class ServerPacketHandler;
class MainServer
{
public:
	MainServer();
	~MainServer();

	bool Start(int port);
	void HandleListener(fd_set& readSet) const;
	void HandleRecv(fd_set& readSet);
	void HandleSend(fd_set& writeSet);
	void Update();

	SOCKET ListenerSocket;
	ServerPacketHandler* PacketHandler;

};

