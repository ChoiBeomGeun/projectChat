#include "SessionManager.h"
//=================================================================================================
// @brief 세션 등록
//=================================================================================================
void SessionManager::RegisterSession(Session * session,string nameKey)
{
    Utility::HandleError((CheckSessionExist(nameKey) == true),
        "Exist Name! : " + nameKey);

    SessionMap[nameKey] = session;
}
//=================================================================================================
// @brief 세션 삭제
//=================================================================================================
void SessionManager::RemoveSessionMap(string nameKey)
{
    Utility::HandleError((CheckSessionExist(nameKey) == true),
        "Not Exist Name! : " + nameKey);

    SessionMap.erase(nameKey);

}
//=================================================================================================
// @brief 단일 메시지 전송 함수
//=================================================================================================
void SessionManager::SendSingleMessage(const char* msg,string nameKey)
{
    Session * session = GetSectionWithKey(nameKey);
    send(session->socket, msg, sizeof(msg), 0);
}
//=================================================================================================
// @brief Key를 이용해 세션을 받아오는 함수
//=================================================================================================
Session* SessionManager::GetSectionWithKey(string nameKey)
{
    auto result = SessionMap.find(nameKey);

    Utility::HandleError(result == SessionMap.end(),
        "Not Exist Name! : " + nameKey);

    return result->second;
}
//=================================================================================================
// @brief Key를 이용해 세션이 있는지 확인하는 함수
//=================================================================================================
bool SessionManager::CheckSessionExist(string key)
{
    return SessionMap.find(key) != SessionMap.end();
}
//=================================================================================================
// @brief 전역메시지 전송을 위한 브로드 캐스팅 함수
//=================================================================================================
void SessionManager::BroadcastMessage(const char* msg)
{
    for (std::pair<string, Session*> element : SessionMap)
    {
        SendSingleMessage(msg, element.first);
    }
}
