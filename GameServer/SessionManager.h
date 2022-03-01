#pragma once
#include "pch.h"
#include "Session.h"
//=================================================================================================
// @brief 세션들을 관리하기 위한 클래스
//=================================================================================================

class SessionManager
{
	friend class MainServer;
	friend class ClientManager;
public:
	void RegisterSession(SOCKET socket, string key);
	void RemoveSessionMap(string ipKey);
	void BroadcastMessage(const string& msg);
	void SendSingleMessage(const string& msg,string ipKey);

	Session* GetSectionWithKey(string ipKey);
private:
	bool CheckSessionExist(string ipKey);
	void SendSingleMessageWithSession(const string & msg, Session * session);

	//session hashmap for searching
	unordered_map<string,Session*> SessionMap;
};


extern SessionManager GSessionManager;
