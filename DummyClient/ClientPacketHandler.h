#pragma once
#include "pch.h"

enum
{
	Login
};
//=================================================================================================
// @brief Ŭ���̾�Ʈ ��Ŷ ����Ŭ������
//=================================================================================================
class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, unsigned len);
	// temp
	static void Handle_Login_TEST(BYTE* buffer, unsigned len);
};

