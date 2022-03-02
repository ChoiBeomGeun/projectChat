#pragma once
#include "pch.h"
class Session;
class Client;
class ClientManager
{
public:
	~ClientManager();
	void RegisterClient(Session* session, const string & nameKey);
	void RemoveClient(const string & ipKey);
	void ShowClientList(const Session * session);

	Client* GetClientWithNameKey(const string & nameKey);
	Client* GetClientWithIpKey(const string & ipKey);

	bool CheckClientExist(const string & nameKey);
	bool CheckClientExistWithIpKey(const string & ipKey);
	pair<bool,Client*> TryGetClientWithIpKey(const string& ipKey);

private:
	//session hashmap for searching
	unordered_map<string, Client*> ClientMap;
	unordered_map<string, Client*> ClientIpMap;
};
extern ClientManager GClientManager;
