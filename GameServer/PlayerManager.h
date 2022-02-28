#pragma once
#include "CoreMacro.h"
#include "pch.h"
class PlayerManager
{
	USE_LOCK;

public:
	void PlayerThenAccount();
	void Lock();
};

extern PlayerManager GPlayerManager;
