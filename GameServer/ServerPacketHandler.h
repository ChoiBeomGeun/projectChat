#pragma once
#include "pch.h"
#include "Session.h"

enum
{
	Login
};
//=================================================================================================
// @brief 서버 패킷 헬퍼클래ㅐ스
//=================================================================================================


class ServerPacketHandler
{
public:
	using HandleFunc = std::function<void(ServerPacketHandler&, vector<string>, Session*)>;
	ServerPacketHandler();

	void ProcessInput(Session & session);
	// Handle command Functions
private:
	void HandleLogin(vector<string> args, Session* session);

	void HandleCreateRoom(vector<string> args, Session* session);
	void HandleEnterRoom(vector<string> args, Session* session);
	void HandleDestroyRoom(vector<string> args, Session* session);
	void HandleExitRoom(vector<string> args, Session* session);

	void HandleUserList(vector<string> args, Session* session);
	void HandleRoomList(vector<string> args, Session* session);
	void HandleRoomUserList(vector<string> args, Session* session);

	void HandleWhisper(vector<string> args, Session* session);

	void HandleCommandHelp(vector<string> args, Session* session);

	bool ValidateLoginArgs(vector<string>& args);
	bool ValidateCreateRoomArgs(vector<string>& args);
	bool ValidateEnterRoomArgs(vector<string>& args);
	bool ValidateDestroyRoomArgs(vector<string>& args);
	bool ValidateWhisperArgs(vector<string>& args);
	bool ValidateRoomUserListArgs(vector<string>& args);
private:
	//unordered_map{command name , tuple {need args count, function object}}
	unordered_map<string, tuple<int, HandleFunc>> CommandList;
};

