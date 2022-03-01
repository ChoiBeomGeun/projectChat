#include "RoomManager.h"

#include "SessionManager.h"

#include "StringTable.h"

#include <format>
//=================================================================================================
// @brief ���ο� �� ��� (�ٷ� ����)
//=================================================================================================
void RoomManager::RegisterRoom(Client * owner, string roomName, int maxRoomCount)
{
	Room* room = new Room(*owner,roomName,maxRoomCount);
    RoomList.push_back(room);

    GSessionManager.SendSingleMessage(StringTable::AlarmCreatingRoom,owner->GetSession()->Key);

    //������ �濡 �ٷ� ����
    EnterRoom(owner,static_cast<int>(RoomList.size()) -1);
}
//=================================================================================================
// @brief �� ���� ��Ŷ
//=================================================================================================
void RoomManager::EnterRoom(Client * client, int roomNumber)
{
    if(RoomList.size() <=0)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNoRoom, client->GetSession()->Key);
        return;
    }
    client->SetRoomState(roomNumber);
    Room * room = GetRoomWithNumber(roomNumber);
    room->GetClients().push_back(*client);
    string notifyMessage = format(StringTable::AlarmEnterRoom, client->GetName(), room->GetCurUserCount(), room->GetMaxRoomCount());
    BroadCastToRoom(room, notifyMessage);
}
//=================================================================================================
// @brief �� ������
//=================================================================================================
void RoomManager::ExitRoom(Client* client)
{
    if(client->GetEntertedRoomNumber() == -1)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNotInRoom, client->GetSession()->Key);
        return;
    }

    Room* room = GetRoomWithNumber(client->GetEntertedRoomNumber());
    vector<Client> clientList = room->GetClients();

    //Notify clients after erasing container
    clientList.erase(std::remove_if(clientList.begin(), clientList.end(), [=](Client pClient) {return pClient.GetName() == client->GetName(); }), clientList.end());
    client->SetRoomState(-1);

    string notifyMessage = format(StringTable::AlarmExitRoom, client->GetName(), room->GetCurUserCount(), room->GetMaxRoomCount());
    BroadCastToRoom(room, notifyMessage);
    
}

//=================================================================================================
// @brief �� ����
//=================================================================================================
void RoomManager::DestroyRoom(int roomNumber,Session * session)
{
    if (CheckRoomExistByNumber(roomNumber) == false)
    {
        GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription,session->Key);
        return;
    }

    Room* room = GetRoomWithNumber(roomNumber);
    vector<Client> clientList = room->GetClients();



    //Notify clients before deleting room
    string notifyMessage = format(StringTable::AlarmDestroyRoom, room->GetRoomName());
    BroadCastToRoom(room, notifyMessage);

    for (Client client : clientList)
    {
        client.SetRoomState(-1);
    }

  // RoomList.erase(std::remove_if(RoomList.begin(), RoomList.end(), room), RoomList.end());
    delete(room);
}
//=================================================================================================
// @brief �� ��ȣ�� �̿��� Room �޾ƿ��� �Լ�
//=================================================================================================
Room* RoomManager::GetRoomWithNumber(int roomNumber)
{
    int idx = 0;
    for (Room* element : RoomList)
    {
        if (idx == roomNumber)
        {
            return element;
        }
        idx++;
    }

    return nullptr;
}
//=================================================================================================
// @brief room number�� �̿��� Room�� �����ϴ��� Ȯ���ϴ� �Լ�
//=================================================================================================
bool RoomManager::CheckRoomExistByNumber(int roomNumber)
{
    return (RoomList.size() > 0) && (roomNumber <= RoomList.size() - 1);
}

//=================================================================================================
// @brief �� ����Ʈ���� ����ϴ� �Լ�
//=================================================================================================
void RoomManager::ShowRoomList(Session * session)
{
    if (RoomList.size() <= 0)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNoRoom, session->Key);
        return;
    }

    std::stringstream ss;

    int roomNumber = 0;
    for (Room* element : RoomList)
    {
        string userCount =std::format("({}/{})", std::to_string(element->GetCurUserCount()), std::to_string(element->GetMaxRoomCount()));
        string format = std::format(StringTable::TemplateRoomList,roomNumber,element->GetRoomName() , element->GetOwner().GetName(), userCount);
		ss << format;
    }
    string result = ss.str();
    GSessionManager.SendSingleMessage(result, session->Key);

}
//=================================================================================================
// @brief ��ȿ� Ŭ���̾�Ʈ�鿡�� �޽��� �����ϴ� �Լ�
//=================================================================================================
void RoomManager::BroadCastToRoom(Room* room, string& msg)
{
    for(Client client : room->GetClients())
    {
        GSessionManager.SendSingleMessage(msg, client.GetSession()->Key);
    }
}
//=================================================================================================
// @brief ��ȿ� Ŭ���̾�Ʈ�鿡�� �޽��� �����ϴ� �Լ�
//=================================================================================================
void RoomManager::BroadCastToRoomWithNumber(int roomNumber, string& msg)
{
    Room* room = GetRoomWithNumber(roomNumber);
    BroadCastToRoom(room, msg);

}

