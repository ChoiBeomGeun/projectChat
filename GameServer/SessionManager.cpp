#include "SessionManager.h"
#include "Session.h"
//=================================================================================================
// @brief 세션 등록
//=================================================================================================
void SessionManager::RegisterSession(SOCKET socket, string key)
{
    //중복된 ip 체크
    Utility::HandleError((CheckSessionExist(key) == true),
        "Exist Name! : " + key);


    Session* session = new Session{ socket, key };

    SessionMap[session->Key] = session;
}
//=================================================================================================
// @brief 세션 삭제
//=================================================================================================
void SessionManager::RemoveSessionMap(string ipKey)
{
    //세션이 존재하는지 체크
    Utility::HandleError((CheckSessionExist(ipKey) == false),
        "Not Exist Name! : " + ipKey);

    delete(GetSectionWithKey(ipKey));

    SessionMap.erase(ipKey);

}
//=================================================================================================
// @brief 단일 메시지 전송 함수
//=================================================================================================
void SessionManager::SendSingleMessage(const string& msg,string ipKey)
{
    Session * session = GetSectionWithKey(ipKey);
    SendSingleMessageWithSession(msg, session);
    session->Reset();
}

//=================================================================================================
// @brief Key를 이용해 세션을 받아오는 함수
//=================================================================================================
Session* SessionManager::GetSectionWithKey(string ipKey)
{
    auto result = SessionMap.find(ipKey);

    Utility::HandleError(result == SessionMap.end(),
        "Not Exist Name! : " + ipKey);

    return result->second;
}
//=================================================================================================
// @brief Key를 이용해 세션이 있는지 확인하는 함수
//=================================================================================================
bool SessionManager::CheckSessionExist(string ipKey)
{
    return SessionMap.find(ipKey) != SessionMap.end();
}
//=================================================================================================
// @brief 전역메시지 전송을 위한 브로드 캐스팅 함수
//=================================================================================================
void SessionManager::BroadcastMessage(const string& msg)
{
    for (std::pair<string, Session*> element : SessionMap)
    {
        SendSingleMessage(msg, element.first);
    }

}

//=================================================================================================
// @brief 세션을 이용한 메시지 전송 함수
//=================================================================================================
void SessionManager::SendSingleMessageWithSession(const string & msg, Session* session)
{
    send(session->Socket, msg.c_str(),static_cast<int>(msg.size()),0);
    std::cout << msg << endl;
}

