#include "SessionManager.h"
#include "ClientManager.h"
#include "Session.h"

SessionManager::~SessionManager()
{
    for (pair<string, Session*> pair : SessionMap)
    {
        delete(pair.second);
    }
}

//=================================================================================================
// @brief ���� ���
//=================================================================================================
void SessionManager::RegisterSession(const SOCKET & socket,const string & key)
{
    Session* session = new Session{ socket, key };

    SessionMap[session->Key] = session;
}
//=================================================================================================
// @brief ���� ����
//=================================================================================================
void SessionManager::RemoveSessionMap(const string & ipKey)
{
    //������ �����ϴ��� üũ
    Utility::HandleError((CheckSessionExist(ipKey) == false),
        "Not Exist Name! : " + ipKey);

    Session* session = GetSectionWithKey(ipKey);
    //delete client Info
    if (GClientManager.CheckClientExistWithIpKey(session->Key)) GClientManager.RemoveClient(session->Key);
    SessionMap.erase(ipKey);
	delete(session);

    shutdown(session->Socket, SD_SEND);
    closesocket(session->Socket);
}
//=================================================================================================
// @brief ���� �޽��� ���� �Լ�
//=================================================================================================
void SessionManager::SendSingleMessage(const string& msg,const string & ipKey)
{
    Session * session = GetSectionWithKey(ipKey);
    SendSingleMessageWithSession(msg, session);
    session->Reset();
}

unordered_map<string, Session*>& SessionManager::GetSessionMap()
{
    return SessionMap;
}

//=================================================================================================
// @brief Key�� �̿��� ������ �޾ƿ��� �Լ�
//=================================================================================================
Session* SessionManager::GetSectionWithKey(const string & ipKey)
{
    auto result = SessionMap.find(ipKey);

    Utility::HandleError(result == SessionMap.end(),
        "Not Exist Name! : " + ipKey,false);

    return result->second;
}
//=================================================================================================
// @brief Key�� �̿��� ������ �ִ��� Ȯ���ϴ� �Լ�
//=================================================================================================
bool SessionManager::CheckSessionExist(const string & ipKey)
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
void SessionManager::SendSingleMessageWithSession(const string & msg,const Session* session)
{
    send(session->Socket, msg.c_str(),static_cast<int>(msg.size()),0);
    std::cout << msg << endl;
}

