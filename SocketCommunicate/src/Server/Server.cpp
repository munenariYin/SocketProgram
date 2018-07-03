#include "winsock.h"
#include <vector>
#include <thread>
#include <map>
#include "../CommunicateThread/CommunicateThread.h"
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
	if (!this->acceptSocket.Bind(AF_INET, INADDR_ANY, 49152)) return false;
	if (!this->acceptSocket.Listen(SOMAXCONN)) return false;

	this->portNo = _portNo;
	this->acceptSocket.SetBlocking(false);

	return true;
}

void Server::Update()
{
	while (true)
	{
		Socket clientSocket;
		if (!acceptSocket.Accept(clientSocket)) return;
		
		CreateClientThread(clientSocket);
		if (this->isEnd) return;
	}
}

bool Server::BeginAccept()
{
	//std::thread* acceptThread = new std::thread(&Server::AcceptThread, this);
	//acceptThread->detach();

	//return false;
}

void Server::AcceptThread()
{
	while (true)
	{

	}
}



void CommunicationThread(Socket& _socket, bool& _isEnd)
{
	while (true)
	{
		std::string data;
		const int MSG_SIZE = 1024;
		int len = _socket.Receive(data, MSG_SIZE, 0);
		// ⑦メッセージ受信
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(_socket.GetSocket(), "recv()");
		}
		printf("受信:%s\n", data.c_str());
		// ⑥メッセージ送信
		len = _socket.Send(data, data.length(), 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(_socket.GetSocket(), "send()");
		}
		printf("送信:%s\n", data);

		if (data == "bye")
		{
			return;
		}
		if (data == "end")
		{
			_isEnd = true;
		}
		if (_isEnd)
		{
			return;
		}
	}
}

void Server::CreateClientThread(Socket& _socket)
{
	//CommunicateThread 
	//communicate.insert(_socket, )
	communicateSockets.push_back(_socket);
	
	std::thread* thread = std::move(new std::thread(&Server::CommunicateThread, this, _socket));
	thread->detach();
}

void Server::CommunicateThread(Socket& _socket)
{
	while (true)
	{
		std::string data;
		const int MSG_SIZE = 1024;
		int len = _socket.Receive(data, MSG_SIZE, 0);
		// ⑦メッセージ受信
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(_socket.GetSocket(), "recv()");
		}
		printf("受信:%s\n", data.c_str());
		// ⑥メッセージ送信
		len = _socket.Send(data, data.length(), 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(_socket.GetSocket(), "send()");
		}
		printf("送信:%s\n", data);

		if (data == "bye")
		{
			return;
		}
		if (data == "end")
		{
			this->isEnd = true;
		}
		if (isEnd)
		{
			return;
		}
	}
}



void Server::Release()
{
	this->acceptSocket.Release();
	for (int i = this->communicateSockets.size() - 1; i >= 0; i--)
	{
		this->communicateSockets[i].Release();
		this->communicateSockets.erase(this->communicateSockets.begin() + i);
	}
	WSACleanup();
}

