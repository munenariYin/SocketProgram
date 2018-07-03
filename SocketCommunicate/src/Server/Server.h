#ifndef __Server_h__
#define __Server_h__

#include "../Socket/Socket.h"

class Server
{
public:
	Server();
	~Server(){}

	bool Init(unsigned short _portNo);
	void Update();

	bool BeginAccept();
	void Release();

private:
	void AcceptThread();
	void CreateClientThread(Socket& _socket);
	void CommunicateThread(Socket& _socket);

private:
	WSADATA wsaData;
	Socket acceptSocket;
	std::vector<Socket> communicateSockets;
//	std::map<Socket, CommunicateThread> communicate;
	int threadCnt = 0;
	unsigned short portNo;
	bool isEnd = false;

};

#endif