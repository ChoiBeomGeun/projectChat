#include "pch.h"
#include "Utility.h"
//=================================================================================================
// @brief wchar ���� char�� �ٲٱ� ���� �Լ�
//		  ���� ���ɼ� ����
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
// @brief ������ ���Ʈ�� ���� �Լ�
//=================================================================================================
void Utility::HandleError(bool isError, std::string msg, bool isAssert)
{
	if(isError)
	{
		cout << "Error ! Msg :" << msg;
		if (isAssert) assert(true);
	}

}
