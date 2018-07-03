#include"winsock.h"
#include<string>

#include"Socket.h"
#include"SocketUtil.h"

// ソケットの生成。プロトコル、
bool Socket::CreateSocket(int _protocolFamily, int _type, int _protocol)
{
	this->sock = socket(_protocolFamily, _type, _protocol);
	if (this->sock == INVALID_SOCKET)
	{
		SocketUtil::SockError(sock, "socket()");
		return false;
	}
	return true;
}

// バインド
bool Socket::Bind(short addressFamily, unsigned long _inAddr, unsigned short _portNo)
{
	memset(&this->addr, 0, sizeof this->addr);	// ゼロクリア
	this->addr.sin_family = addressFamily;				// アドレスファミリ
	this->addr.sin_addr.s_addr = htonl(_inAddr);		// IPアドレス
	this->addr.sin_port = htons(_portNo);				// ポート番号

	// ソケットに関連付ける
	if (bind(this->sock, (struct sockaddr*)& this->addr, sizeof this->addr) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->sock, "bind()");
		return false;
	}
	return true;
}

// 待ち行列の生成
bool Socket::Listen(int _backLog)
{
	if (listen(this->sock, _backLog) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->sock, "listen()");
		return false;
	}
	return true;
}

bool Socket::Connect()
{
	// ③サーバに接続を要求
	if (connect(this->sock, (struct sockaddr*)&this->addr, sizeof this->addr) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->sock, "connect()");
		return false;
	}
}

bool Socket::Accept(Socket& _targetSock)
{
	sockaddr_in clientAddr;
	int len = sizeof clientAddr;

	_targetSock.sock = accept(this->sock, (struct sockaddr*)&clientAddr, &len);
	if (_targetSock.sock == INVALID_SOCKET)	return false;
	_targetSock.addr = addr;
}

int Socket::Receive(std::string& _recvData,  int _recvSize, int _flags)
{
	// ひとまずchar*で文字列を受け取るしかない
	char* recvString = 0;
	int recvLength = recv(this->sock, recvString, _recvSize, _flags);
	// 受け取った文字列をstringのデータに追加する。
	_recvData += recvString;
	return recvLength;
}

int Socket::Send(std::string& _sendData, int _sendSize, int _flags)
{
	return send(this->sock, _sendData.c_str(), _sendSize, _flags);
}

void Socket::SetBlocking(bool _isBlocking)
{
	unsigned long val;
	// ブロッキングすると0に、そうでなければ0以外に
	if (_isBlocking) val = 0;
	else val = 1;
	ioctlsocket(this->sock, FIONBIO, &val);
}

void Socket::Release()
{
	closesocket(this->sock);
}
