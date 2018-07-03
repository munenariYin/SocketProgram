#include "ClientSocket.h"

#include"../SocketUtil.h"
#include<stdio.h>
bool ClientSocket::SetUp(int _addressFamily, int _type, int _protocol)
{
	if (!CreateSocket(_addressFamily, _type, _protocol))
	{
		return false;
	}
	return true;
}

bool ClientSocket::Connect()
{
	// ③サーバに接続を要求
	if (connect(this->connectSocket, (struct sockaddr*)&this->serverAddr, sizeof this->serverAddr) == SOCKET_ERROR)
	{
		SocketUtil::SockError(connectSocket, "connect()");
		return false;
	}
	const int MSG_SIZE = 4098;
	char msg[MSG_SIZE + 1];
	while (true)
	{
		int len;
		// ④メッセージ受信
		len = recv(connectSocket, msg, MSG_SIZE, 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(connectSocket, "recv()");
			break;
		}
		msg[len] = '\0';	// 文字列終端処理
		printf("受信:　%s\n", msg);	// 受信メッセージの表示
		if (strncmp(msg, "bye", 3) == 0) break; // 会話終了判定

		printf("入力: ");
		scanf("%s", msg);	// 送信メッセージ入力

							// ⑤メッセージ送信
		len = send(connectSocket, msg, (int)strlen(msg), 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(connectSocket, "send()");
			break;
		}
		printf("送信: %s\n", msg);	// 送信メッセージ表示
	}
	return true;
}

bool ClientSocket::SendInfomation(std::string _type, std::string _values)
{
	return false;
}

bool ClientSocket::RecvInfomation(std::string & _outString)
{
	return false;
}

void ClientSocket::Release()
{
	closesocket(connectSocket);
}



bool ClientSocket::CreateSocket(int _addressFamily, int _type, int _protocol)
{
	this->connectSocket = socket(_addressFamily, _type, _protocol);
	if (this->connectSocket == INVALID_SOCKET)
	{
		SocketUtil::SockError(this->connectSocket, "socket()");
		return false;
	}
	char host[1024 + 1];
	printf("Server IP Address : ");
	scanf("%s", host);		// 接続したいサーバのIPアドレスを入力

	memset(&this->serverAddr, 0, sizeof this->serverAddr);	// ゼロクリア
	this->serverAddr.sin_family = AF_INET;				// アドレスファミリー
	this->serverAddr.sin_addr.s_addr = SocketUtil::TransHostAddr(host);	// サーバのIPアドレス
	this->serverAddr.sin_port = htons(this->portNo);			// サーバのポート番号
	return true;
}
