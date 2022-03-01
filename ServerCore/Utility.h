#pragma once

class Utility
{
public:
	static void HandleError(bool isError, std::string msg, bool isAssert = true);
	static std::vector<std::string> SplitString(char* target, const char* delimiter);
	static bool TryParseIntFromString(int& number, std::string rawString);
};



