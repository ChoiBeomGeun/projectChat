#include "ClientManager.h"

#include "SessionManager.h"
#include "StringTable.h"
#include "Client.h"
#include "RoomManager.h"
#include "Session.h"

ClientManager::~ClientManager()
{
    for(pair<string,Client*> pair : ClientMap)
    {
        delete(pair.second);
    }
}

//=================================================================================================
// @brief 클라이언트 등록
//=================================================================================================
void ClientManager::RegisterClient(Session* session, const string & nameKey)
{
    if(Utility::HandleError((CheckClientExist(nameKey) == true),
        "Exist Name! : " + nameKey,false,session))
    {
        return;
    }



    Client * client = new Client(nameKey, session);
    ClientMap[nameKey] = client;
    ClientIpMap[session->Key] = client;
}

void ClientManager::RemoveClient(const string & ipKey)
{
    Client* client = GetClientWithIpKey(ipKey);

    if(client == nullptr)
    {
        cout << "Client is nullptr , key: " << ipKey << endl;
        return;
    }

    if(client->GetEntertedRoomNumber() != -1)
    {
        GRoomManager.ExitRoom(client);
    }

    ClientIpMap.erase(ipKey);
    ClientMap.erase(client->GetName());
    delete(client);
}


void ClientManager::ShowClientList(const Session * session)
{
    if (ClientIpMap.size() <= 0)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNoRoom, session->Key);
        return;
    }

	stringstream ss;

    for (std::pair<string, Client*> element : ClientIpMap)
    {
        string userInfo = std::format(StringTable::TemplateClientInfo,element.second->GetName() ,element.second->GetSession()->Key);
        ss << userInfo;
    }
    string result = ss.str();
    GSessionManager.SendSingleMessage(result, session->Key);

}

Client* ClientManager::GetClientWithNameKey(const string & nameKey)
{
    auto result = ClientMap.find(nameKey);

    Utility::HandleError(result == ClientMap.end(),
        "Not Exist Name! : " + nameKey,false);

    return result->second;
}
Client* ClientManager::GetClientWithIpKey(const string & ipKey)
{
    auto result = ClientIpMap.find(ipKey);

    Utility::HandleError(result == ClientIpMap.end(),
        "Not Exist Name! : " + ipKey,false);

    return result->second;
}

bool ClientManager::CheckClientExist(const string & nameKey)
{
    return ClientMap.find(nameKey) != ClientMap.end();
}

bool ClientManager::CheckClientExistWithIpKey(const string & ipKey)
{
    return ClientIpMap.find(ipKey) != ClientIpMap.end();
}

pair<bool, Client*> ClientManager::TryGetClientWithIpKey(const string& ipKey)
{
    unordered_map<string, Client*>::iterator findClient = ClientIpMap.find(ipKey);
    pair<bool, Client*> pair;
    if(findClient == ClientIpMap.end())
    {
        pair.first = false;
    }
    else
    {
        pair.first = true;
        pair.second = (*findClient).second;
    }

    return pair;
}
