#pragma once
#include "pch.h"

enum
{
	Login
};
//=================================================================================================
// @brief 클라이언트 패킷 헬퍼클래ㅐ스
//=================================================================================================
class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, unsigned len);
	// temp
	static void Handle_Login_TEST(BYTE* buffer, unsigned len);
};

