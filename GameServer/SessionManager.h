#pragma once
#include "pch.h"

//=================================================================================================
// @brief 세션들을 관리하기 위한 클래스
//=================================================================================================

class Session;
class SessionManager
{
	friend class ClientManager;
public:
	~SessionManager();
	void RegisterSession(const SOCKET & socket,const string & key);
	void RemoveSessionMap(const string & ipKey);
	void BroadcastMessage(const string& msg);
	void SendSingleMessage(const string& msg,const string & ipKey);
	unordered_map<string, Session*> & GetSessionMap();
	Session* GetSectionWithKey(const string & ipKey);
	bool CheckSessionExist(const string & ipKey);
	void SendSingleMessageWithSession(const string & msg,const Session & session);

	//session hashmap for searching

	unordered_map<string,Session*> SessionMap;
};


extern SessionManager GSessionManager;
