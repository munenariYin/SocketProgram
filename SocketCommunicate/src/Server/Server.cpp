#include "winsock.h"
#include <vector>

#include <functional>
#include "Server.h"

#include"../Socket/SocketUtil.h"

Server::Server()
{
	this->threadCnt = 0;
}

bool Server::Init(unsigned short _portNo)
{
	unsigned short version = MAKEWORD(1,1);
	if (WSAStartup(version, &this->wsaData) != 0)
	{
	//	fprintf(stderr, "Failed in initialize.\n");
		return false;
	}
	// accept用ソケットの作成
	if (!this->acceptSocket.CreateSocket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) return false;
	if (!this->acceptSocket.Bind(AF_INET, INADDR_ANY, _portNo)) return false;
	if (!this->acceptSocket.Listen(SOMAXCONN)) return false;

	this->portNo = _portNo;
	this->acceptSocket.SetBlocking(false);
	return true;
}

void Server::Update()
{
	BeginAccept();
	while (true)
	{
		if (this->isEnd) return;
	}
}

void Server::BeginAccept()
{
	bool& isEnd = this->isEnd;
	Socket& acceptSock = this->acceptSocket;
	std::function<bool()> acceptProc = [this, &isEnd, &acceptSock]()
	{
		while (true)
		{
			Socket newClient;
			if (!acceptSock.Accept(newClient))
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK) return false;
				continue;
			}
			if (isEnd)return true;
			std::string hello = "hello";
			newClient.Send(hello, hello.length(), 0);
			this->CreateClientThread(newClient);
		}
		return true;
	};

	// 受付の開始
	this->acceptThread = std::async(std::launch::async, acceptProc);
}


void Server::CreateClientThread(Socket& _socket)
{	
	// 新しいクライアントの用意
	SockThread* clientThread = new SockThread();
	clientThread->SetSocket(_socket);
	bool& isTerminateServer = this->isEnd;
	// スレッドで動かす関数
	std::function<SockErrorType()> communicateProc = [this, &isTerminateServer, clientThread]()
	{
		while (true)
		{
			Socket& clientSocket = clientThread->GetSocket();
			std::string data = "";
			int len = clientSocket.Receive(data, Socket::MSG_MAX, 0);
			// ⑦メッセージ受信
			if (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SocketUtil::SockError(clientSocket.GetSocket(), "recv()");
			}
			printf("受信:%s\n", data.c_str());
			// ⑥メッセージ送信
			len = clientSocket.Send(data, data.length(), 0);
			if (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SocketUtil::SockError(clientSocket.GetSocket(), "send()");
			}	
			printf("送信:%s\n", data.c_str());

			if (data == "bye")
			{
				clientThread->EndThread();
				break;
			}
			if (data == "end")
			{
				isTerminateServer = true;
				clientThread->EndThread();
				break;
			}
			if (isTerminateServer)
			{
				clientThread->EndThread();
				break;
			}
		}
		return SockErrorType::COMPLETE;
	};
	// スレッド生成
	auto communicateThread = std::async(std::launch::async, communicateProc);
	clientThread->SetThread(communicateThread);
	this->clientList.push_back(clientThread);
}

void Server::Release()
{
	//this->acceptThread.get();
	this->acceptSocket.Release();

	for (auto client : this->clientList)
	{
		client->Join();
		delete client;
	}
	clientList.clear();
	WSACleanup();
}

