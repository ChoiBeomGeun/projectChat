#pragma once
#include "pch.h"
class Session;
class Client;
class ClientManager
{
	friend class MainServer;
public:
	void RegisterClient(Session* session, string nameKey);
	void RemoveClient(string ipKey);
	void BroadcastMessage(string& msg);
	void SendSingleMessage(string& msg , string ipKey);
	void ShowClientList(const Session * session);

	Client* GetClientWithNameKey(string nameKey);
	Client* GetClientWithIpKey(string ipKey);

	bool CheckClientExist(string nameKey);
	bool CheckClientExistWithIpKey(string ipKey);

	//session hashmap for searching
	unordered_map<string, Client*> ClientMap;
	unordered_map<string, Client*> ClientIpMap;
};
extern ClientManager GClientManager;
