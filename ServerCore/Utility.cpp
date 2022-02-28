#include "pch.h"
#include "Utility.h"
//=================================================================================================
// @brief wchar 형을 char로 바꾸기 위한 함수
//		  삭제 가능성 있음
//=================================================================================================
char* Utility::wcharTochar(wchar_t* str)
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}
//=================================================================================================
// @brief 에러시 어썰트를 위한 함수
//=================================================================================================
void Utility::HandleError(bool isError, std::string msg, bool isAssert)
{
	if(isError)
	{
		cout << "Error ! Msg :" << msg;
		if (isAssert) assert(true);
	}

}
