#pragma once

#include "pch.h"
class MainServer
{
public:
	MainServer();
	~MainServer();

	bool Start(int port);
	void Update();

	SOCKET ListenerSocket;

};

