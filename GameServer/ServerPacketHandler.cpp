#pragma once
#include "pch.h"
#include "ServerPacketHandler.h"
#include "ClientManager.h"
#include "RoomManager.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringTable.h"
//=================================================================================================
// @brief command 관련 연관 함수 연결 추가
//=================================================================================================
ServerPacketHandler::ServerPacketHandler()
{
	CommandList["/login"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleLogin));
	CommandList["/l"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleLogin));

	CommandList["/createroom"] = make_tuple(2, HandleFunc(&ServerPacketHandler::HandleCreateRoom));
	CommandList["/cr"] = make_tuple(2, HandleFunc(&ServerPacketHandler::HandleCreateRoom));

	CommandList["/enterroom"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleEnterRoom));
	CommandList["/er"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleEnterRoom));

	CommandList["/destroyroom"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleDestroyRoom));
	CommandList["/dr"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleDestroyRoom));

	CommandList["/whisper"] = make_tuple(2, HandleFunc(&ServerPacketHandler::HandleWhisper));
	CommandList["/w"] = make_tuple(2, HandleFunc(&ServerPacketHandler::HandleWhisper));

	CommandList["/roomlist"] = make_tuple(0, HandleFunc(&ServerPacketHandler::HandleRoomList));
	CommandList["/rl"] = make_tuple(0, HandleFunc(&ServerPacketHandler::HandleRoomList));

	CommandList["/roomuserlist"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleRoomUserList));
	CommandList["/rul"] = make_tuple(1, HandleFunc(&ServerPacketHandler::HandleRoomUserList));

	CommandList["/userlist"] = make_tuple(0, HandleFunc(&ServerPacketHandler::HandleUserList));
	CommandList["/ul"] = make_tuple(0, HandleFunc(&ServerPacketHandler::HandleUserList));

	CommandList["/help"] = make_tuple(0, HandleFunc(&ServerPacketHandler::HandleCommandHelp));
	CommandList["/?"] = make_tuple(0, HandleFunc(&ServerPacketHandler::HandleCommandHelp));

}

//=================================================================================================
// @brief 유저 인풋처리 함수
//=================================================================================================
void ServerPacketHandler::ProcessInput(Session & session)
{

	string input(session.RecvBuffer);
	input.replace(input.find("\r\n"), 2, "");
	//if user is in room, just send message
	if (GClientManager.CheckClientExistWithIpKey(session.Key) == true)
	{
		Client* client = GClientManager.GetClientWithIpKey(session.Key);
		//check user is in chat room
		int enteredRoomNumber = client->GetEntertedRoomNumber();
		if(enteredRoomNumber != -1)
		{
			if(input.compare("/exit") == 0)
			{
				GRoomManager.ExitRoom(client);
			}
			else
			{
				string msg = format("[{}] : {}\r\n", client->GetName(), input);
				GRoomManager.BroadCastToRoomWithNumber(enteredRoomNumber, msg);
			}

			return;
		}
	}
	
	vector<string> args = Utility::SplitString(const_cast<char*>(input.c_str()), " ");

	//check args is empty
	if (args.empty() == true)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription, session.Key);
		session.Reset();
		return;
	}

	//make command string to lowercase
	std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);

	string command = args[0];

	auto tuple = CommandList.find(command);

	//check exist command
	if (tuple == CommandList.end())
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription, session.Key);
		session.Reset();
		return;
	}

	int needArgs = std::get<0>(tuple->second);

	//deduce command args count for checking needed args 
	if (needArgs != args.size() - 1)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription, session.Key);
		session.Reset();
		return;
	}

	//Call Registered function
	auto handleFunc = std::get<1>(tuple->second);
	handleFunc(*this, args, &session);
	session.Reset();
	
}
//=================================================================================================
// @brief 로그인 핸들러 함수
// args0 = commandName		(string)
// args1 = name				(string)
//=================================================================================================
void ServerPacketHandler::HandleLogin(vector<string> args, Session* session)
{
	if(ValidateLoginArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription, session->Key);
		return;
	}

	GClientManager.RegisterClient(session, args[1]);

	//클라이언트에게 커맨드관련 안내 전송
	GSessionManager.SendSingleMessage(StringTable::CommandDescription, session->Key);

	//모든 클라이언트들에게 로그인 사실을 알림
	string notifyMessage = format(StringTable::AlarmEnterServer, args[1]);
	GSessionManager.BroadcastMessage(notifyMessage);
	session->Reset();
}

//=================================================================================================
// @brief 방 생성 핸들러 함수
// args0 = commandName		(string)
// args1 = roomName (string)
// args2 = maxUserCount (int)
//=================================================================================================
void ServerPacketHandler::HandleCreateRoom(vector<string> args, Session* session)
{
	if (ValidateCreateRoomArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidCreateRoom, session->Key);
		return;
	}

	string roomName = args[1];
	int	maxUserCount = 1;
	Utility::TryParseIntFromString(OUT maxUserCount,args[2]);
	GRoomManager.RegisterRoom(GClientManager.GetClientWithIpKey(session->Key), args[1], maxUserCount);

}
//=================================================================================================
// @brief 방 입장 핸들러 함수
// args0 = commandName		(string)
// args1 = roomNumber (int)
//=================================================================================================
void ServerPacketHandler::HandleEnterRoom(vector<string> args, Session* session)
{
	if (ValidateEnterRoomArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidCreateRoom, session->Key);
		return;
	}

	string roomName = args[1];
	int	roomNumber = 1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	GRoomManager.EnterRoom(GClientManager.GetClientWithIpKey(session->Key),roomNumber);
}

//=================================================================================================
// @brief 방 파괴 핸들러 함수 (방주인만 파괴가능)
// args0 = commandName		(string)
// args1 = roomNumber (int)
//=================================================================================================
void ServerPacketHandler::HandleDestroyRoom(vector<string> args, Session* session)
{
	if (ValidateDestroyRoomArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidDestroyRoom, session->Key);
		return;
	}

	int	roomNumber = -1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	GRoomManager.DestroyRoom(roomNumber, session);
}
//=================================================================================================
// @brief 방 나가기 핸들러 함수
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleExitRoom(vector<string> args, Session* session)
{
	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	int enteredRoomNumber = client->GetEntertedRoomNumber();
	if(enteredRoomNumber == -1)
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmNotInRoom, session->Key);
	}
	else
	{
		GRoomManager.ExitRoom(client);
	}
}

//=================================================================================================
// @brief 유저 목록 핸드러 함수
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleUserList(vector<string> args, Session* session)
{
	GClientManager.ShowClientList(session);
}
//=================================================================================================
// @brief 방 목록 핸들러 함수
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleRoomList(vector<string> args, Session* session)
{
	GRoomManager.ShowRoomList(session);
}
//=================================================================================================
// @brief 방 유저 목록 핸들러 함수
// args0 = commandName		(string)
// args1 = roomNumber		(string)
//=================================================================================================
void ServerPacketHandler::HandleRoomUserList(vector<string> args, Session* session)
{
	if (ValidateRoomUserListArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidDestroyRoom, session->Key);
		return;
	}
	int	roomNumber = -1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	GRoomManager.ShowRoomUserList(session, roomNumber);
}
//=================================================================================================
// @brief 귓속말 핸들러 함수
// args0 = commandName		(string)
// args1 = userName			(string)
// args2 = message			(string)
//=================================================================================================
void ServerPacketHandler::HandleWhisper(vector<string> args, Session* session)
{
	if (ValidateWhisperArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidWhisper,session->Key);
		return;
	}

	// 나의 클라이언트 정보
	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	// 귓속말 상대 정보
	Client* opClient = GClientManager.GetClientWithNameKey(args[1]);
	string msg = format(StringTable::TemplateWhispher, client->GetName()) + args[2];
	GSessionManager.SendSingleMessage(msg, opClient->GetSession()->Key);
	GSessionManager.SendSingleMessage(StringTable::AlarmSendWhispher, session->Key);
}
//=================================================================================================
// @brief 명령어 헬프 함수
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleCommandHelp(vector<string> args, Session* session)
{
	GSessionManager.SendSingleMessage(StringTable::CommandDescription, session->Key);
}
//=================================================================================================
// @brief Validate LoginArgs
// args0 = commandName
// args1 = name
//=================================================================================================
bool ServerPacketHandler::ValidateLoginArgs(vector<string>& args)
{
	return true;
}
//=================================================================================================
// @brief Validate CreateRoom Args
// args0 = commandName	(string)
// args1 = roomName		(string)
// args2 = maxUserCount (int)
//=================================================================================================
bool ServerPacketHandler::ValidateCreateRoomArgs(vector<string>& args)
{
	int maxUserCount = -1;
	bool result = Utility::TryParseIntFromString(OUT maxUserCount, args[2]);
	if (result == false) return false;
	return maxUserCount > 1;
	
}
//=================================================================================================
// @brief Validate EnterRoom Args
// args0 = commandName	(string)
// args1 = roomNumber   (int)
//=================================================================================================
bool ServerPacketHandler::ValidateEnterRoomArgs(vector<string>& args)
{
	int roomNumber = -1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	if (GRoomManager.CheckRoomExistByNumber(roomNumber) == false) return false;
	return true;
}

//=================================================================================================
// @brief Validate DestroyRoom Args
// args0 = commandName	(string)
//=================================================================================================
bool ServerPacketHandler::ValidateDestroyRoomArgs(vector<string>& args)
{
	int roomNumber = -1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	if (GRoomManager.CheckRoomExistByNumber(roomNumber) == false) return false;
	return true;
}
//=================================================================================================
// @brief Validate Whisper Args
// args0 = commandName	(string)
// args1 = userName (string)
// args2 = msg		(string)
//=================================================================================================
bool ServerPacketHandler::ValidateWhisperArgs(vector<string>& args)
{
	if (GClientManager.CheckClientExist(args[1]) == false) return false;
	return true;
}
//=================================================================================================
// @brief Validate RoomUserListArgs
// args0 = commandName	(string)
// args1 = roomNumber   (int)
//=================================================================================================
bool ServerPacketHandler::ValidateRoomUserListArgs(vector<string>& args)
{
	int roomNumber = -1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	if (GRoomManager.CheckRoomExistByNumber(roomNumber) == false) return false;
	return true;
}


