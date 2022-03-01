#include "ClientManager.h"

#include "SessionManager.h"
#include "StringTable.h"
//=================================================================================================
// @brief 클라이언트 등록
//=================================================================================================
void ClientManager::RegisterClient(Session* session, string nameKey)
{
    Utility::HandleError((CheckClientExist(nameKey) == true),
        "Exist Name! : " + nameKey);

    Client* client = new Client(nameKey, session);
    ClientMap[nameKey] = client;
    ClientIpMap[session->Key] = client;

    string notifyMessage = format(StringTable::AlarmEnterServer, nameKey);
    GSessionManager.BroadcastMessage(notifyMessage);
}

void ClientManager::RemoveClient(string ipKey)
{
}

void ClientManager::BroadcastMessage(string& msg)
{
}

//=================================================================================================
// @brief 단일 메시지 전송 함수
//=================================================================================================
void ClientManager::SendSingleMessage(string & msg, string nameKey)
{
    Client* client = GetClientWithNameKey(nameKey);

    GSessionManager.SendSingleMessageWithSession(msg, client->GetSession());
}

void ClientManager::ShowClientList(Session * session)
{
    if (ClientIpMap.size() <= 0)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNoRoom, session->Key);
        return;
    }

    std::stringstream ss;


    for (std::pair<string, Client*> element : ClientIpMap)
    {
        string userInfo = std::format("유저 닉네임 : {} 유저 주소 : {} \r\n",element.second->GetName() ,element.second->GetSession()->Key);
        ss << userInfo;
    }
    string result = ss.str();
    GSessionManager.SendSingleMessage(result, session->Key);

}

Client* ClientManager::GetClientWithNameKey(string nameKey)
{
    auto result = ClientMap.find(nameKey);

    Utility::HandleError(result == ClientMap.end(),
        "Not Exist Name! : " + nameKey);

    return result->second;
}
Client* ClientManager::GetClientWithIpKey(string ipKey)
{
    auto result = ClientIpMap.find(ipKey);

    Utility::HandleError(result == ClientIpMap.end(),
        "Not Exist Name! : " + ipKey);

    return result->second;
}

bool ClientManager::CheckClientExist(string nameKey)
{
    return ClientMap.find(nameKey) != ClientMap.end();
}

bool ClientManager::CheckClientExistWithIpKey(string ipKey)
{
    return ClientIpMap.find(ipKey) != ClientIpMap.end();
}
