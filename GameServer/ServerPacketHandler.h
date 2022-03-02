#pragma once
#include "pch.h"

class Session;
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
	using HandleFunc = std::function<void(ServerPacketHandler&,const vector<string>&, Session*)>;
	ServerPacketHandler();

	void ProcessInput(Session & session);
	// Handle command Functions
private:
	void HandleLogin(const vector<string> & args, Session* session);

	void HandleCreateRoom(const vector<string>& args,const Session* session);
	void HandleEnterRoom(const vector<string> &args, const Session* session);
	void HandleDestroyRoom(const vector<string>& args, const Session* session);
	void HandleExitRoom(const vector<string>& args, const Session* session);

	void HandleUserList(const vector<string> &args, const Session* session);
	void HandleRoomList(const vector<string>& args, const Session* session);
	void HandleRoomUserList(const vector<string>& args, const Session* session);

	void HandleWhisper(const vector<string> &args, const Session* session);

	void HandleCommandHelp(const vector<string> &args, const Session* session);

	bool ValidateLoginArgs(const vector<string>& args);
	bool ValidateCreateRoomArgs(const vector<string>& args);
	bool ValidateEnterRoomArgs(const vector<string>& args);
	bool ValidateDestroyRoomArgs(const vector<string>& args);
	bool ValidateWhisperArgs(const vector<string>& args);
	bool ValidateRoomUserListArgs(const vector<string>& args);
private:
	//unordered_map{command name , tuple {if true, command execute in chat room,need args count, function object}}
	unordered_map<string, tuple<bool,int, HandleFunc>> CommandList;
};

