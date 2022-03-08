#pragma once
#include <iostream>

struct RequestPacketHeader
{
	int packetSize; // 공용 헤더
	int packetId; // 공용 헤더
};

struct ResponsePacketHeader
{
	int packetSize; // 공용 헤더
	int packetId; // 공용 헤더
	char* errorMsg;
};
struct RequestLogin : RequestPacketHeader
{
	char* name;


};

struct ResponseLogin : ResponsePacketHeader
{
	
};

