#include "SessionManager.h"
//=================================================================================================
// @brief ���� ���
//=================================================================================================
void SessionManager::RegisterSession(Session * session,string nameKey)
{
    Utility::HandleError((CheckSessionExist(nameKey) == true),
        "Exist Name! : " + nameKey);

    SessionMap[nameKey] = session;
}
//=================================================================================================
// @brief ���� ����
//=================================================================================================
void SessionManager::RemoveSessionMap(string nameKey)
{
    Utility::HandleError((CheckSessionExist(nameKey) == true),
        "Not Exist Name! : " + nameKey);

    SessionMap.erase(nameKey);

}
//=================================================================================================
// @brief ���� �޽��� ���� �Լ�
//=================================================================================================
void SessionManager::SendSingleMessage(const char* msg,string nameKey)
{
    Session * session = GetSectionWithKey(nameKey);
    send(session->socket, msg, sizeof(msg), 0);
}
//=================================================================================================
// @brief Key�� �̿��� ������ �޾ƿ��� �Լ�
//=================================================================================================
Session* SessionManager::GetSectionWithKey(string nameKey)
{
    auto result = SessionMap.find(nameKey);

    Utility::HandleError(result == SessionMap.end(),
        "Not Exist Name! : " + nameKey);

    return result->second;
}
//=================================================================================================
// @brief Key�� �̿��� ������ �ִ��� Ȯ���ϴ� �Լ�
//=================================================================================================
bool SessionManager::CheckSessionExist(string key)
{
    return SessionMap.find(key) != SessionMap.end();
}
//=================================================================================================
// @brief �����޽��� ������ ���� ��ε� ĳ���� �Լ�
//=================================================================================================
void SessionManager::BroadcastMessage(const char* msg)
{
    for (std::pair<string, Session*> element : SessionMap)
    {
        SendSingleMessage(msg, element.first);
    }
}
