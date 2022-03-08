#pragma once
#include <iostream>

struct RequestPacketHeader
{
	int packetSize; // ���� ���
	int packetId; // ���� ���
};

struct ResponsePacketHeader
{
	int packetSize; // ���� ���
	int packetId; // ���� ���
	char* errorMsg;
};
struct RequestLogin : RequestPacketHeader
{
	char* name;


};

struct ResponseLogin : ResponsePacketHeader
{
	
};

