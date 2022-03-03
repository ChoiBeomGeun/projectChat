#include "pch.h"
#include "Utility.h"

#include "Session.h"
#include "SessionManager.h"

//=================================================================================================
// @brief 스트링 분리 헬퍼 함수
//=================================================================================================
vector<string> Utility::SplitString(char* target, const char* delimiter,int maxSplit)
{
	char* context = NULL;
	char* token = strtok_s(target, delimiter, &context);
	vector<string> splitStrings;

	
	while (token != NULL)
	{
		if(splitStrings.size() + 1 == maxSplit)
		{
			string lastString = format("{0} {1}", token, context);
			splitStrings.push_back(lastString);
			break;
		}

		splitStrings.push_back(token);
		token = strtok_s(NULL, delimiter, &context);
	}
	return splitStrings;
}


//=================================================================================================
// @brief 에러시 어썰트를 위한 함수
// 세션정보가 넘어오고 에러가 났을때 메시지 전송
//=================================================================================================
bool Utility::HandleError(bool isError,const std::string & msg, bool isAssert ,const Session * notifySession )
{
	cout << "";
	if (isError)
	{
		cout << "Error ! Msg :" << msg << endl;
		if (isAssert) assert(TRUE);
	}

	if(notifySession != nullptr && isError)
	{
		GSessionManager.SendSingleMessageWithSession(msg, notifySession);
	}

	return isError;
}

//=================================================================================================
// @brief 스트링을 int로 변환하기 위한 함수
// OUT   : converted number from rawString
//RETURN : parse result
//=================================================================================================
bool Utility::TryParseIntFromString(OUT int& targetNumber, std::string rawString)
{
	stringstream intParse(rawString);
	intParse >> targetNumber;
	if (intParse.fail()) return false;
	return true;
}

