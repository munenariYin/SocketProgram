
#include "ServerSocket.h"

#include"../SocketUtil.h"
bool ServerSocket::SetUp(unsigned short _majorVer, unsigned short _minorVer)
{
	unsigned short requestVersion = MAKEWORD(_majorVer, _minorVer);
	if (WSAStartup(requestVersion, &this->wsaData) != 0)
	{
		fprintf(stderr, "Failed in initialize.\n");
		return false;
	}
	if (!CreateSocket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) return false;
	if(!BindSocket())
	{
		return false;
	}
	
	return true;
}

bool ServerSocket::Connect()
{

	return false;
}

bool ServerSocket::SendInfomation(string _type, string _values)
{
	return false;
}

bool ServerSocket::RecvInfomation(string & _outString)
{
	return false;
}

bool ServerSocket::CreateSocket(int _addressFamily, int _type, int _protocol)
{
	this->acceptSocket = socket(_addressFamily, _type, _protocol);
	if (this->acceptSocket == INVALID_SOCKET)
	{
		SocketUtil::SockError(this->acceptSocket, "socket()");
		return false;
	}
	return true;
}

// 未接続のソケットに対して行う
bool ServerSocket::BindSocket()
{
	memset(&this->serverAddr, 0, sizeof this->serverAddr);	// ゼロクリア
	this->serverAddr.sin_family = AF_INET;					// アドレスファミリ
	this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// IPアドレス
	this->serverAddr.sin_port = htons(this->portNo);		// ポート番号

	// ソケットに関連付ける
	if (bind(this->acceptSocket, (struct sockaddr*)& this->serverAddr, sizeof this->serverAddr) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->acceptSocket, "bind()");
		return false;
	}

	return true;
}

bool ServerSocket::ListenSocket()
{
	return false;
}
