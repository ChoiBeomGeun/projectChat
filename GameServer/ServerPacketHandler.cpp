#pragma once
#include "pch.h"
#include "ServerPacketHandler.h"
#include "ClientManager.h"
#include "RoomManager.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringTable.h"
#include "Client.h"

//=================================================================================================
// @brief command ���� ���� �Լ� ���� �߰�
//=================================================================================================
ServerPacketHandler::ServerPacketHandler()
{
	CommandList["/login"] = make_tuple(false,1, HandleFunc(&ServerPacketHandler::HandleLogin));
	CommandList["/l"] = make_tuple(false, 1, HandleFunc(&ServerPacketHandler::HandleLogin));

	CommandList["/createroom"] = make_tuple(false, 2, HandleFunc(&ServerPacketHandler::HandleCreateRoom));
	CommandList["/cr"] = make_tuple(false, 2, HandleFunc(&ServerPacketHandler::HandleCreateRoom));

	CommandList["/enterroom"] = make_tuple(false, 1, HandleFunc(&ServerPacketHandler::HandleEnterRoom));
	CommandList["/er"] = make_tuple(false, 1, HandleFunc(&ServerPacketHandler::HandleEnterRoom));

	CommandList["/destroyroom"] = make_tuple(false, 1, HandleFunc(&ServerPacketHandler::HandleDestroyRoom));
	CommandList["/dr"] = make_tuple(false, 1, HandleFunc(&ServerPacketHandler::HandleDestroyRoom));

	CommandList["/invite"] = make_tuple(true, 1, HandleFunc(&ServerPacketHandler::HandleInviteRoom));
	CommandList["/i"] = make_tuple(true, 1, HandleFunc(&ServerPacketHandler::HandleInviteRoom));

	CommandList["/exit"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleExitRoom));

	CommandList["/whisper"] = make_tuple(true, 2, HandleFunc(&ServerPacketHandler::HandleWhisper));
	CommandList["/w"] = make_tuple(true, 2, HandleFunc(&ServerPacketHandler::HandleWhisper));

	CommandList["/roomlist"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleRoomList));
	CommandList["/rl"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleRoomList));

	CommandList["/roomuserlist"] = make_tuple(true, 1, HandleFunc(&ServerPacketHandler::HandleRoomUserList));
	CommandList["/rul"] = make_tuple(true,1, HandleFunc(&ServerPacketHandler::HandleRoomUserList));

	CommandList["/userlist"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleUserList));
	CommandList["/ul"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleUserList));

	CommandList["/help"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleCommandHelp));
	CommandList["/?"] = make_tuple(true, 0, HandleFunc(&ServerPacketHandler::HandleCommandHelp));

}

//=================================================================================================
// @brief ���� ��ǲó�� �Լ�
//=================================================================================================
void ServerPacketHandler::ProcessInput(Session & session)
{
	bool isClientInChatRoom = false;
	string input(session.RecvBuffer);
	input.replace(input.find("\r\n"), 2, "");

	//handle shut down input
	if (input == "/Q" || input == "/q")
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmExitSession, session.Key);
		GSessionManager.RemoveSessionMap(session.Key);
		return;
	}
		

	//if user is in room, just send message
	if (GClientManager.CheckClientExistWithIpKey(session.Key) == true)
	{
		Client* client = GClientManager.GetClientWithIpKey(session.Key);
		if (client == nullptr)
		{
			cout << "Client is nullptr , key: " << session.Key << endl;
			return;
		}
		//check user is in chat room
		int enteredRoomNumber = client->GetEntertedRoomNumber();
		if (enteredRoomNumber != -1) isClientInChatRoom = true;
		//user is in chat room and not use command
		if(enteredRoomNumber  != -1 &&input[0] != '/')
		{
			isClientInChatRoom = true;
	
			string msg = format("[{}] : {}\r\n", client->GetName(), input);
			GRoomManager.BroadCastToRoomWithNumber(enteredRoomNumber, msg);
			session.Reset();
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

	//check command  can be called in chat room
	bool isExecuteInChatRoom = std::get<0>(tuple->second);

	if (isClientInChatRoom == true && isExecuteInChatRoom == false)
	{
		session.Reset();
		return ;
	}
	int needArgs = std::get<1>(tuple->second);

	//deduce command args count for checking needed args 
	if (needArgs != args.size() - 1)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription, session.Key);
		session.Reset();
		return;
	}

	//Call Registered function
	auto handleFunc = std::get<2>(tuple->second);
	handleFunc(*this, args, &session);
	session.Reset();
}
//=================================================================================================
// @brief �α��� �ڵ鷯 �Լ�
// args0 = commandName		(string)
// args1 = name				(string)
//=================================================================================================
void ServerPacketHandler::HandleLogin(const vector<string>& args, Session* session)
{
	if(ValidateLoginArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidInputDescription, session->Key);
		return;
	}

	if(GClientManager.CheckClientExistWithIpKey(session->Key) == true)
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmExistSession, session->Key);
		return;
	}

	GClientManager.RegisterClient(session, args[1]);

	//Ŭ���̾�Ʈ���� Ŀ�ǵ���� �ȳ� ����
	GSessionManager.SendSingleMessage(StringTable::CommandDescription, session->Key);

	//��� Ŭ���̾�Ʈ�鿡�� �α��� ����� �˸�
	string notifyMessage = format(StringTable::AlarmEnterServer, args[1]);
	GSessionManager.BroadcastMessage(notifyMessage);
	session->Reset();
}

//=================================================================================================
// @brief �� ���� �ڵ鷯 �Լ�
// args0 = commandName		(string)
// args1 = roomName (string)
// args2 = maxUserCount (int)
//=================================================================================================
void ServerPacketHandler::HandleCreateRoom(const vector<string>& args, const Session* session)
{

	if (ValidateCreateRoomArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidCreateRoom, session->Key);
		return;
	}

	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	if (client == nullptr)
	{
		cout << "Client is nullptr , key: " << session->Key << endl;
		return;
	}

	string roomName = args[1];
	int	maxUserCount = 1;
	Utility::TryParseIntFromString(OUT maxUserCount,args[2]);
	GRoomManager.RegisterRoom(client, args[1], maxUserCount);

}
//=================================================================================================
// @brief �� ���� �ڵ鷯 �Լ�
// args0 = commandName		(string)
// args1 = roomNumber (int)
//=================================================================================================
void ServerPacketHandler::HandleEnterRoom(const vector<string>& args, const Session* session)
{
	if (ValidateEnterRoomArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::InvalidCreateRoom, session->Key);
		return;
	}

	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	if (client == nullptr)
	{
		cout << "Client is nullptr , key: " << session->Key << endl;
		return;
	}
	string roomName = args[1];
	int	roomNumber = 1;
	Utility::TryParseIntFromString(OUT roomNumber, args[1]);
	GRoomManager.EnterRoom(client,roomNumber);
}

//=================================================================================================
// @brief �� �ı� �ڵ鷯 �Լ� (�����θ� �ı�����)
// args0 = commandName		(string)
// args1 = roomNumber (int)
//=================================================================================================
void ServerPacketHandler::HandleDestroyRoom(const vector<string>& args, const Session* session)
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
// @brief �� ������ �ڵ鷯 �Լ�
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleExitRoom(const vector<string>& args, const Session* session)
{
	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	if (client == nullptr)
	{
		cout << "Client is nullptr , key: " << session->Key << endl;
		return;
	}
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
// @brief ���� �ʴ� �ڵ巯 �Լ�
// args0 = commandName		(string)
// args1 = oppenentName		(string)
//=================================================================================================
void ServerPacketHandler::HandleInviteRoom(const vector<string>& args, const Session* session)
{
	if (ValidateInviteRoomArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmNoClient, session->Key);
		return;
	}
	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	if(client->GetEntertedRoomNumber() == -1)
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmNotInRoom, session->Key);
	}
	else
	{
		// ���� Ŭ���̾�Ʈ ����
		Client* client = GClientManager.GetClientWithIpKey(session->Key);
		// �ʴ� ��� ����
		Client* opClient = GClientManager.GetClientWithNameKey(args[1]);

		if(client == opClient)
		{
			GSessionManager.SendSingleMessage(StringTable::AlarmNoClient, session->Key);
		}
		else
		{
			string msg = format(StringTable::TemplateInvite, client->GetName(), client->GetEntertedRoomNumber());
			GSessionManager.SendSingleMessage(msg, opClient->GetSession()->Key);
			GSessionManager.SendSingleMessage(StringTable::AlarmSendInvite, session->Key);
		}
	}
}

//=================================================================================================
// @brief ���� ��� �ڵ巯 �Լ�
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleUserList(const vector<string>& args, const Session* session)
{
	GClientManager.ShowClientList(session);
}
//=================================================================================================
// @brief �� ��� �ڵ鷯 �Լ�
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleRoomList(const vector<string>& args, const Session* session)
{
	GRoomManager.ShowRoomList(session);
}
//=================================================================================================
// @brief �� ���� ��� �ڵ鷯 �Լ�
// args0 = commandName		(string)
// args1 = roomNumber		(string)
//=================================================================================================
void ServerPacketHandler::HandleRoomUserList(const vector<string>& args, const Session* session)
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
// @brief �ӼӸ� �ڵ鷯 �Լ�
// args0 = commandName		(string)
// args1 = userName			(string)
// args2 = message			(string)
//=================================================================================================
void ServerPacketHandler::HandleWhisper(const vector<string>& args, const Session* session)
{
	if (ValidateWhisperArgs(args) == false)
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmNoClient, session->Key);
	}

	// ���� Ŭ���̾�Ʈ ����
	Client* client = GClientManager.GetClientWithIpKey(session->Key);
	// �ӼӸ� ��� ����
	Client* opClient = GClientManager.GetClientWithNameKey(args[1]);
	if (client == opClient)
	{
		GSessionManager.SendSingleMessage(StringTable::AlarmSameSession, session->Key);
	}
	else
	{
		string msg = format(StringTable::TemplateWhispher, client->GetName()) + args[2];
		GSessionManager.SendSingleMessage(msg, opClient->GetSession()->Key);
		GSessionManager.SendSingleMessage(StringTable::AlarmSendWhispher, session->Key);
	}
}
//=================================================================================================
// @brief ��ɾ� ���� �Լ�
// args0 = commandName		(string)
//=================================================================================================
void ServerPacketHandler::HandleCommandHelp(const vector<string>& args, const Session* session)
{
	pair<bool, Client*> result = GClientManager.TryGetClientWithIpKey(session->Key);
	if(result.first)
	{
		if(result.second->GetEntertedRoomNumber() == -1)
		{
			GSessionManager.SendSingleMessage(StringTable::CommandDescription, session->Key);
		}
		else
		{
			GSessionManager.SendSingleMessage(StringTable::RoomCommandDescription, session->Key);
		}
	}
	else
	{
		GSessionManager.SendSingleMessage(StringTable::CommandDescription, session->Key);
	}
}
//=================================================================================================
// @brief Validate LoginArgs
// args0 = commandName
// args1 = name
//=================================================================================================
bool ServerPacketHandler::ValidateLoginArgs(const vector<string>& args)
{
	return true;
}
//=================================================================================================
// @brief Validate CreateRoom Args
// args0 = commandName	(string)
// args1 = roomName		(string)
// args2 = maxUserCount (int)
//=================================================================================================
bool ServerPacketHandler::ValidateCreateRoomArgs(const vector<string>& args)
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
bool ServerPacketHandler::ValidateEnterRoomArgs(const vector<string>& args)
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
bool ServerPacketHandler::ValidateDestroyRoomArgs(const vector<string>& args)
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
bool ServerPacketHandler::ValidateWhisperArgs(const vector<string>& args)
{
	if (GClientManager.CheckClientExist(args[1]) == false) return false;
	return true;
}
//=================================================================================================
// @brief Validate RoomUserListArgs
// args0 = commandName	(string)
// args1 = roomNumber   (int)
//=================================================================================================
bool ServerPacketHandler::ValidateRoomUserListArgs(const vector<string>& args)
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
//=================================================================================================
bool ServerPacketHandler::ValidateInviteRoomArgs(const vector<string>& args)
{
	if (GClientManager.CheckClientExist(args[1]) == false) return false;
	return true;
}


