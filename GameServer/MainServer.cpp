#include "MainServer.h"

#include "ClientManager.h"
#include "ServerPacketHandler.h"
#include "Session.h"
#include "SessionManager.h"

#include "StringTable.h"
const int BUFSIZE = 4096;


void ProcessInput(Session  session);

MainServer::MainServer()
{
	
	


}

MainServer::~MainServer()
{

}



//=================================================================================================
// @brief 서버 초기화
//=================================================================================================
bool MainServer::Start(int port)
{
	//윈도우 소켓 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup Failed!" << endl;
		assert(true);
	}
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	Utility::HandleError(listenSocket == INVALID_SOCKET,
		"Invalid Socket!");

	ListenerSocket = listenSocket;

	u_long on = 1;


	//set unblocking
	::ioctlsocket(listenSocket, FIONBIO, &on);

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(port);

	Utility::HandleError((::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR),
	"Bind Failed !");

	Utility::HandleError((::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR),
		"Listen Failed !");


	cout << "Server is Initialized!\n" << "Open Port : " << port;
	return true;
}
//=================================================================================================
// @brief 서버 메인 루프
//=================================================================================================
void MainServer::Update()
{

	ServerPacketHandler packetHandler;
	while (true)
	{
		fd_set reads;
		fd_set writes;

		FD_ZERO(&reads);
		FD_ZERO(&writes);

		FD_SET(ListenerSocket, &reads);

		for (std::pair<string, Session*> element : GSessionManager.SessionMap)
		{
			Session* s = element.second;
	
			FD_SET(s->Socket, &reads);
		}

		Utility::HandleError((int)(::select(0, &reads, &writes, nullptr, nullptr) == SOCKET_ERROR),
			"Select Socket Failed !");
	
		// Listener 
		if (FD_ISSET(ListenerSocket, &reads))
		{

			SOCKADDR_IN clientAddr;
			int addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(ListenerSocket, (SOCKADDR*)&clientAddr, &addrLen);

			char buf[32];
			auto ip = inet_ntop(AF_INET, &clientAddr, buf, sizeof(buf));
		
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;

				string key = string(ip);
			
				GSessionManager.RegisterSession( clientSocket, key );
				GSessionManager.SendSingleMessage(StringTable::LoginDescription, key);

			}
		}


		for(unordered_map<string, Session*>::iterator it = GSessionManager.SessionMap.begin(); 
			it != GSessionManager.SessionMap.end();)
		{
			Session* s = it->second;
			if (FD_ISSET(s->Socket, &reads))
			{
				int recvLen = ::recv(s->Socket, s->RecvBuffer + s->RecvBytes, BUFSIZE - s->RecvBytes, 0);

				if (recvLen <= 0)
				{
					// session 및 client 객체 제거

					if (GClientManager.CheckClientExistWithIpKey(s->Key)) GClientManager.RemoveClient(s->Key);
					GSessionManager.RemoveSessionMap(s->Key);
					break;
				}

				s->RecvBytes += recvLen;

				if (s->RecvBuffer[s->RecvBytes - 1] == '\n')
				{
					packetHandler.ProcessInput(*s);

					/*	FD_SET(s->Socket, &writes);*/
				}

			}

			// Write
			else if (FD_ISSET(s->Socket, &writes))
			{


			}
			++it;
		}


	}
}





