#include "RoomManager.h"
#include "Client.h"
#include "SessionManager.h"
#include  "Session.h"
#include "StringTable.h"
#include "Room.h"
#include <format>

RoomManager::~RoomManager()
{
    for(Room * room : RoomList)
    {
        delete(room);
    }
}

//=================================================================================================
// @brief 새로운 방 등록 (바로 입장)
//=================================================================================================
void RoomManager::RegisterRoom(Client * owner, string roomName, int maxRoomCount)
{
	Room* room = new Room(owner,roomName,maxRoomCount);
    RoomList.emplace_back(room);

    GSessionManager.SendSingleMessage(StringTable::AlarmCreatingRoom,owner->GetSession()->Key);

    //생성한 방에 바로 입장
    EnterRoom(owner,static_cast<int>(RoomList.size()) -1);
}
//=================================================================================================
// @brief 방 입장 패킷
//=================================================================================================
void RoomManager::EnterRoom(Client * client, int roomNumber)
{
    if(RoomList.size() <=0)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNoRoom, client->GetSession()->Key);
        return;
    }
 
    Room * room = GetRoomWithNumber(roomNumber);

    //최대 입장 유저 수 확인
    if(room->GetCurUserCount() +1 > room->GetMaxRoomCount())
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmFullRoom, client->GetSession()->Key);
        return;
    }
    GSessionManager.SendSingleMessage(std::format(StringTable::AlarmEnterSelfRoom,room->GetRoomName(),std::format("{}/{}", std::to_string(room->GetCurUserCount()), std::to_string(room->GetMaxRoomCount()))), client->GetSession()->Key);
    client->SetRoomState(roomNumber);
    room->AddClient(client);
    string notifyMessage = format(StringTable::AlarmEnterRoom, client->GetName(), room->GetCurUserCount(), room->GetMaxRoomCount());
    BroadCastToRoom(room, notifyMessage);
}
//================================================================================================= 
// @brief 방 나가기
//=================================================================================================
void RoomManager::ExitRoom(Client* client)
{
    if(client->GetEntertedRoomNumber() == -1)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNotInRoom, client->GetSession()->Key);
        return;
    }

    int roomNumber = client->GetEntertedRoomNumber();

    Room* room = GetRoomWithNumber(roomNumber);

    //Notify clients before erasing container
    GSessionManager.SendSingleMessage(StringTable::AlarmExitSelfRoom, client->GetSession()->Key);
    
    string notifyMessage = format(StringTable::AlarmExitRoom, client->GetName(), room->GetCurUserCount() -1, room->GetMaxRoomCount());
    BroadCastToRoom(room, notifyMessage);

    room->RemoveClient(client);
   
    client->SetRoomState(-1);


    if(room->GetCurUserCount() == 0)
    {
        DestroyRoom(roomNumber,client->GetSession());
    }
    
}

//=================================================================================================
// @brief 방 삭제
//=================================================================================================
void RoomManager::DestroyRoom(int roomNumber,const Session * session)
{
    if (CheckRoomExistByNumber(roomNumber) == false)
    {
        GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription,session->Key);
        return;
    }

    Room* room = GetRoomWithNumber(roomNumber);
    vector<Client*> clientList = room->GetClients();
    string notifyMessage = format(StringTable::AlarmDestroyRoom, room->GetRoomName());
    //Notify client before deleting room
    GSessionManager.SendSingleMessage(notifyMessage,session->Key);

    for (Client * client : clientList)
    {
        client->SetRoomState(-1);
    }

    RoomList.erase(RoomList.begin()+ roomNumber);
    delete(room);
}
//=================================================================================================
// @brief 방 번호를 이용해 Room 받아오는 함수
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
// @brief room number를 이용해 Room이 존재하는지 확인하는 함수
//=================================================================================================
bool RoomManager::CheckRoomExistByNumber(int roomNumber)
{
    return (RoomList.size() > 0) && (roomNumber <= RoomList.size() - 1);
}

//=================================================================================================
// @brief 방 리스트들을 출력하는 함수
//=================================================================================================
void RoomManager::ShowRoomList(const Session * session)
{
    if (RoomList.size() <= 0)
    {
        GSessionManager.SendSingleMessage(StringTable::AlarmNoRoom, session->Key);
        return;
    }

    std::stringstream ss;

    int roomNumber = 0;
    GSessionManager.SendSingleMessage(StringTable::AlarmRoomList, session->Key);
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
// @brief 해당 방에 어떤 유저가 접속했는지 체크하는 함수
//=================================================================================================
void RoomManager::ShowRoomUserList(const Session * session,int roomNumber)
{
    Room* room = GetRoomWithNumber(roomNumber);
    vector<Client*> clients = room->GetClients();
    std::stringstream ss;
    ss << StringTable::AlarmRoomUserList;
    string userCount = std::format("({}/{})", std::to_string(room->GetCurUserCount()), std::to_string(room->GetMaxRoomCount()));
    string roomDes = std::format(StringTable::TemplateRoomList, roomNumber, room->GetRoomName(), room->GetOwner().GetName(), userCount);
    ss << roomDes;
    for(Client * client : clients)
    {
        string userInfo = std::format(StringTable::TemplateClientInfo, client->GetName(), client->GetSession()->Key);
        ss << userInfo;
    }

    string result = ss.str();
    GSessionManager.SendSingleMessage(result, session->Key);
}

//=================================================================================================
// @brief 방안에 클라이언트들에게 메시지 전송하는 함수
//=================================================================================================
void RoomManager::BroadCastToRoom(Room* room,const string& msg)
{
    for(Client * client : room->GetClients())
    {
        GSessionManager.SendSingleMessage(msg, client->GetSession()->Key);
    }
}
//=================================================================================================
// @brief 방안에 클라이언트들에게 메시지 전송하는 함수
//=================================================================================================
void RoomManager::BroadCastToRoomWithNumber(const int roomNumber,const string& msg)
{
    Room* room = GetRoomWithNumber(roomNumber);
    BroadCastToRoom(room, msg);

}

