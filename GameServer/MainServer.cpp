#include "MainServer.h"

#include "Session.h"
#include "SessionManager.h"


const int BUFSIZE = 4096;


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

	_listenerSocket = listenSocket;

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
	
	return true;
}
//=================================================================================================
// @brief 서버 메인 루프
//=================================================================================================
void MainServer::Update()
{
	vector<Session> sessions;
	sessions.reserve(100);
	while (true)
	{
		fd_set reads;
		fd_set writes;

		FD_ZERO(&reads);
		FD_ZERO(&writes);

		FD_SET(_listenerSocket, &reads);

		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		Utility::HandleError((int)(::select(0, &reads, &writes, nullptr, nullptr) == SOCKET_ERROR),
			"Select Socket Failed !");

		// Listener 
		if (FD_ISSET(_listenerSocket, &reads))
		{

			SOCKADDR_IN clientAddr;
			int addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(_listenerSocket, (SOCKADDR*)&clientAddr, &addrLen);

			char buf[32];
			auto ip = inet_ntop(AF_INET, &clientAddr, buf, sizeof(buf));
		
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;
				sessions.push_back(Session{ clientSocket });

				GSessionManager.RegisterSession(new Session{ clientSocket }, string(ip));

				GSessionManager.SendSingleMessage("닉네임을 입력하시오",string(ip));
				/*wchar_t str[100] = L"안냐세여";
				char* pStr = Utility::wcharTochar(str);
				::send(clientSocket, pStr, sizeof(pStr), 0);*/
			}
		}


		for (std::pair<string, Session*> element : GSessionManager.SessionMap)
		{
			Session * s = element.second;
			if (FD_ISSET(s->socket, &reads))
			{
				int recvLen = ::recv(s->socket, s->recvBuffer + s->recvBytes, BUFSIZE - s->recvBytes, 0);

				if (recvLen <= 0)
				{
					// TODO : sessions 제거
					continue;
				}

				s->recvBytes += recvLen;

				if (s->recvBuffer[s->recvBytes - 1] == '\n')
				{
					cout << "NameSet : " << recvLen << endl;

					string name (s->recvBuffer);
					name.replace(name.find("\r\n"),2,"");
					GSessionManager.BroadcastMessage(name.c_str());
					s->Reset();
				}

				cout << "recv Lng : " << s->recvBytes << endl;
			}
			//TODO : 인풋처리 추가
			// Write
			if (FD_ISSET(s->socket, &writes))
			{

				//int sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				//if (sendLen == SOCKET_ERROR)
				//{		
				//	// TODO : sessions 제거
				//	continue;
				//}

				//s.sendBytes += sendLen;
				//if (s.recvBytes == s.sendBytes)
				//{
				//	s.recvBytes = 0;
				//	s.sendBytes = 0;
				//}

				//cout << "send Lng : " << sendLen << endl;
				//s.Reset();
			}
		}


		}
	}

