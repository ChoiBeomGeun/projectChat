#pragma once
#include "pch.h"
class Session;
class Utility
{
public:
	static bool HandleError(bool isError, const std::string& msg, bool isAssert= true, const Session* notifySession =nullptr);
	static std::vector<std::string> SplitString(char* target, const char* delimiter,int maxSplitCount = -1);
	static bool TryParseIntFromString(int& number, std::string rawString);
};



