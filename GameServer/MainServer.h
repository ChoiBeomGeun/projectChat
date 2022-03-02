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
	void HandleRecv(fd_set& readSet) ;
	void Update();

	SOCKET ListenerSocket;
	ServerPacketHandler* PacketHandler;

};

