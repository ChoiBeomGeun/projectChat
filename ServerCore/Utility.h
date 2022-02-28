#pragma once

class Utility
{
public:
	static char* wcharTochar(wchar_t* str);
	static void HandleError(bool isError, std::string msg, bool isAssert = true);
};

