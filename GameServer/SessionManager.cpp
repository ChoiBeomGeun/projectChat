#include "SessionManager.h"
#include "Session.h"
//=================================================================================================
// @brief ���� ���
//=================================================================================================
void SessionManager::RegisterSession(SOCKET socket, string key)
{
    //�ߺ��� ip üũ
    Utility::HandleError((CheckSessionExist(key) == true),
        "Exist Name! : " + key);


    Session* session = new Session{ socket, key };

    SessionMap[session->Key] = session;
}
//=================================================================================================
// @brief ���� ����
//=================================================================================================
void SessionManager::RemoveSessionMap(string ipKey)
{
    //������ �����ϴ��� üũ
    Utility::HandleError((CheckSessionExist(ipKey) == false),
        "Not Exist Name! : " + ipKey);

    delete(GetSectionWithKey(ipKey));

    SessionMap.erase(ipKey);

}
//=================================================================================================
// @brief ���� �޽��� ���� �Լ�
//=================================================================================================
void SessionManager::SendSingleMessage(const string& msg,string ipKey)
{
    Session * session = GetSectionWithKey(ipKey);
    SendSingleMessageWithSession(msg, session);
    session->Reset();
}

//=================================================================================================
// @brief Key�� �̿��� ������ �޾ƿ��� �Լ�
//=================================================================================================
Session* SessionManager::GetSectionWithKey(string ipKey)
{
    auto result = SessionMap.find(ipKey);

    Utility::HandleError(result == SessionMap.end(),
        "Not Exist Name! : " + ipKey);

    return result->second;
}
//=================================================================================================
// @brief Key�� �̿��� ������ �ִ��� Ȯ���ϴ� �Լ�
//=================================================================================================
bool SessionManager::CheckSessionExist(string ipKey)
{
    return SessionMap.find(ipKey) != SessionMap.end();
}
//=================================================================================================
// @brief �����޽��� ������ ���� ��ε� ĳ���� �Լ�
//=================================================================================================
void SessionManager::BroadcastMessage(const string& msg)
{
    for (std::pair<string, Session*> element : SessionMap)
    {
        SendSingleMessage(msg, element.first);
    }

}

//=================================================================================================
// @brief ������ �̿��� �޽��� ���� �Լ�
//=================================================================================================
void SessionManager::SendSingleMessageWithSession(const string & msg, Session* session)
{
    send(session->Socket, msg.c_str(),static_cast<int>(msg.size()),0);
    std::cout << msg << endl;
}

