#pragma once
#include "pch.h"
#include "SendBuffer.h"

enum
{
	Login
};
//=================================================================================================
// @brief ���� ��Ŷ ����Ŭ������
//=================================================================================================


class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, unsigned len);
	// temp
	static SendBuffer Make_Login_TEST(unsigned id);
};

