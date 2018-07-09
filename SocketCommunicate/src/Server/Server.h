#ifndef __Server_h__
#define __Server_h__

#include "../Socket/Socket.h"
#include <future>
#include <list>

class SockThread
{
public:
	SockThread() {}
	~SockThread()
	{
		sock.Release();
	}

	void SetThread(std::future<SockErrorType>& _thread)
	{
		this->thread = std::move(_thread);
	}
	void SetSocket(Socket& _socket)
	{
		this->sock = _socket;
	}
	Socket& GetSocket()
	{
		return this->sock;
	}
	bool IsThreadEnd()
	{
		return this->isEnd;
	}
	void EndThread()
	{
		this->isEnd = true;
	}
	SockErrorType Join()
	{
		return this->thread.get();
	}

private:
	Socket sock;
	std::future<SockErrorType> thread;
	bool isEnd = false;
};

class Server
{
public:
	Server();
	~Server(){}

	bool Init(unsigned short _portNo);
	void Update();

	void BeginAccept();
	void Release();

	void CreateClientThread(Socket& _socket);

public:
	static const int MAX_MSG_SIZE = 1024;

private:
	WSADATA wsaData;

	Socket acceptSocket;
	std::future<bool> acceptThread;

	std::list<SockThread*> clientList;
	std::vector<Socket> communicateSockets;
	int threadCnt = 0;
	unsigned short portNo;
	bool isEnd = false;

};



#endif