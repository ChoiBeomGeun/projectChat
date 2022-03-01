#pragma once
#include "Client.h"
#include "Session.h"

class ClientManager
{
	friend class MainServer;
public:
	void RegisterClient(Session* session, string nameKey);
	void RemoveClient(string ipKey);
	void BroadcastMessage(string& msg);
	void SendSingleMessage(string& msg , string ipKey);
	void ShowClientList(Session * session);

	Client* GetClientWithNameKey(string nameKey);
	Client* GetClientWithIpKey(string ipKey);

	bool CheckClientExist(string nameKey);
	bool CheckClientExistWithIpKey(string ipKey);

	//session hashmap for searching
	unordered_map<string, Client*> ClientMap;
	unordered_map<string, Client*> ClientIpMap;
};
extern ClientManager GClientManager;
