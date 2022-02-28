#pragma once
#include "pch.h"
#include "SendBuffer.h"

enum
{
	Login
};
//=================================================================================================
// @brief 서버 패킷 헬퍼클래ㅐ스
//=================================================================================================


class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, unsigned len);
	// temp
	static SendBuffer Make_Login_TEST(unsigned id);
};

