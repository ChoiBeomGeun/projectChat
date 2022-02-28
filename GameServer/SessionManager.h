#pragma once
#include "pch.h"
#include "Session.h"
//=================================================================================================
// @brief 세션들을 관리하기 위한 클래스
//=================================================================================================

class SessionManager
{
	friend class MainServer;
public:
	void RegisterSession(Session* session,string name);
	void RemoveSessionMap(string nameKey);
	void BroadcastMessage(const char * msg);
	void SendSingleMessage(const char* msg,string nameKey);
	Session* GetSectionWithKey(string nameKey);
private:
	bool CheckSessionExist(string key);


	//session hashmap for searching
	unordered_map<string,Session*> SessionMap;
};

extern SessionManager GSessionManager;