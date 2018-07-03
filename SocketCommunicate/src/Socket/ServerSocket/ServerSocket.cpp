
#include "ServerSocket.h"

#include"../SocketUtil.h"
#include<thread>
#include<process.h>
#include<stdio.h>
bool ServerSocket::SetUp(int _addressFamily, int _type, int _protocol)
{
	if (!CreateSocket(_addressFamily, _type, _protocol)) return false;
	if(!BindSocket())
	{
		return false;
	}
	if (!ListenSocket())
	{
		return false;
	}
	
	return true;
}

void SockThread(void *_sock)
{
	const int MSG_SIZE = 1024;
	char msg[MSG_SIZE + 1];
	int len;

	SOCKET talkSocket;
	talkSocket = (SOCKET)_sock;

	do
	{
		// ⑦メッセージ受信
		len = recv(talkSocket, msg, MSG_SIZE, 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(talkSocket, "recv()");
			break;
		}
		msg[len] = '\0';
		printf("受信:%s\n", msg);
		// ⑥メッセージ送信
		len = send(talkSocket, msg, (int)strlen(msg), 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(talkSocket, "send()");
			break;
		}
		printf("送信:%s\n", msg);
	} while (strncmp(msg, "bye", 3) != 0);	// 会話終了判定

											// ⑧会話用ソケットを閉じる
	closesocket(talkSocket);
	_endthread();	//スレッドの終了

	return;
}

bool ServerSocket::Connect()
{

	//// マルチスレッド
	//while (true)
	//{
	//	struct sockaddr_in clientAddr;
	//	int len = sizeof clientAddr;

	//	// ⑤接続要求受付
	//	SOCKET talkSocket;


	//	// ⑤接続要求受付
	//	talkSocket = accept(acceptSocket, (struct sockaddr*)&clientAddr, &len);

	//	if (talkSocket == INVALID_SOCKET)
	//	{
	//		SocketUtil::SockError(acceptSocket, "accept()");
	//		break;
	//	}



	//	std::thread communicateThread = [](SOCKET _clientSocket)
	//	{
	//		const int MSG_SIZE = 1024;
	//		char msg[MSG_SIZE + 1];
	//		int len;

	//		SOCKET talkSocket;
	//		talkSocket = (SOCKET)_clientSocket;

	//		do
	//		{
	//			// ⑦メッセージ受信
	//			len = recv(talkSocket, msg, MSG_SIZE, 0);
	//			if (len == SOCKET_ERROR)
	//			{
	//				SocketUtil::SockError(talkSocket, "recv()");
	//			}
	//			msg[len] = '\0';
	//			printf("受信:%s\n", msg);
	//			// ⑥メッセージ送信
	//			len = send(talkSocket, msg, (int)strlen(msg), 0);
	//			if (len == SOCKET_ERROR)
	//			{
	//				SocketUtil::SockError(talkSocket, "send()");
	//			}
	//			printf("送信:%s\n", msg);
	//		} while (strncmp(msg, "bye", 3) != 0);	// 会話終了判定

	//		// ⑧会話用ソケットを閉じる
	//		closesocket(talkSocket);
	//		return;
	//	};
	//	// スレッドの切り離し
	//	communicateThread.detach();

	//	//uintptr_t hThread = _beginthread(SockThread, 0, (void*)talkSocket);

	//	//if (hThread == -1)
	//	//{
	//	//	printf("thread creation error. \n");
	//	//	closesocket(talkSocket);
	//	//	break;
	//	//}

	//	printf("Client	IP	: %s\n", inet_ntoa(clientAddr.sin_addr));	// IPアドレス,network to ASCII
	//	printf("Client	Port: %hu\n", ntohs(clientAddr.sin_port));		// ポート番号,network to host short
	//																	// ⑥メッセージ送信
	//	len = send(talkSocket, "hello", (int)strlen("hello"), 0);
	//	if (len == SOCKET_ERROR)
	//	{
	//		SocketUtil::SockError(talkSocket, "send()");
	//		break;
	//	}
	//}

	//return true;

	fd_set checkFD;
	struct timeval timeOut;
	timeOut.tv_sec = 5;
	timeOut.tv_usec = 0;

	short timeOutCnt = 0;
	const short TIMEOUT_CNT_MAX = 3;

	while (1)
	{
		// fe_setの情報を初期化
		FD_ZERO(&checkFD);	// 毎フレームゼロにクリアする
		// 受付用ソケットのセット
		FD_SET(this->acceptSocket, &checkFD);
		// クライアント用ソケットのセット
		for (auto clientSocket : this->clientVector)
		{
			FD_SET(clientSocket, &checkFD);
		}

		// ソケットに通信が来ているか
		int callSocketCnt = select(
			FD_SETSIZE,	// UNIXとの互換性の為。fd_setの中のsocketをSETSIZE分だけ確認する
			&checkFD,
			NULL,
			NULL,
			&timeOut);
		// タイムアウト時

		if (callSocketCnt == 0)
		{
			timeOutCnt++;
			fprintf(stderr, "select timeout.\n");
			// 複数回タイムアウトが発生すれば終了
			if (timeOutCnt > TIMEOUT_CNT_MAX) return false;
			// もう一度、selectを走らせる
			continue;
		}
		timeOutCnt = 0;

		// accept用ソケットに要求が確認できた場合、クライアント用ソケットを一つ作る
		if (FD_ISSET(this->acceptSocket, &checkFD))
		{
			fprintf(stderr, "request\n");
			struct sockaddr_in addrIn;
			int len = sizeof addrIn;
			SOCKET clientSocket = accept(this->acceptSocket, (struct sockaddr*)& addrIn, &len);
			this->clientVector.push_back(clientSocket);

			len = send(clientSocket, "hello", (int)strlen("hello"), 0);
			if (len == SOCKET_ERROR)
			{
				SocketUtil::SockError(clientSocket, "send()");
			}
			printf("送信:%s\n", "hello");
		}

		// 要求が来ているクライアントとだけ応対する。
		for (int i = this->clientVector.size() - 1; i >= 0; i--)
		{
			if (FD_ISSET(this->clientVector[i], &checkFD))
			{
				const int MSG_SIZE = 1024;
				char msg[MSG_SIZE + 1];
				int len;

				// ⑦メッセージ受信
				len = recv(this->clientVector[i], msg, MSG_SIZE, 0);
				if (len == SOCKET_ERROR)
				{
					SocketUtil::SockError(this->clientVector[i], "recv()");
				}
				msg[len] = '\0';
				printf("受信:%s\n", msg);
				// ⑥メッセージ送信
				len = send(this->clientVector[i], msg, (int)strlen(msg), 0);
				if (len == SOCKET_ERROR)
				{
					SocketUtil::SockError(this->clientVector[i], "send()");
				}
				printf("送信:%s\n", msg);

				if (strncmp(msg, "bye", 3) == 0)
				{
					this->clientVector.erase(clientVector.begin() + i);
				}
			}
		}
		//// 同じソケットなら終了
		//bool isSameSock = false;
		//for(auto& sock : clientVector)
		//{
		//	if (sock == clientSocket)
		//	{
		//		isSameSock = true;
		//		break;
		//	}
		//}
		//if (isSameSock) break;

		// マルチスレッド生成 //////////////////////////////////
		// "bye"が送られたら終了

		//std::thread communicateThread([](SOCKET _clientSocket)
		//{
		//	const int MSG_SIZE = 1024;
		//	char msg[MSG_SIZE + 1];
		//	int len;

		//	SOCKET talkSocket;
		//	talkSocket = (SOCKET)_clientSocket;

		//	do
		//	{
		//		// ⑦メッセージ受信
		//		len = recv(talkSocket, msg, MSG_SIZE, 0);
		//		if (len == SOCKET_ERROR)
		//		{
		//			SocketUtil::SockError(talkSocket, "recv()");
		//		}
		//		msg[len] = '\0';
		//		printf("受信:%s\n", msg);
		//		// ⑥メッセージ送信
		//		len = send(talkSocket, msg, (int)strlen(msg), 0);
		//		if (len == SOCKET_ERROR)
		//		{
		//			SocketUtil::SockError(talkSocket, "send()");
		//		}
		//		printf("送信:%s\n", msg);
		//	} while (strncmp(msg, "bye", 3) != 0);	// 会話終了判定

		//	// ⑧会話用ソケットを閉じる
		//	closesocket(talkSocket);
		//	return;
		//});
		//// スレッドの切り離し
		//communicateThread.detach();

		///////////////////////////////////////////////////////////

//		break;
	}
	return true;
}

bool ServerSocket::SendInfomation(std::string _type, std::string _values)
{
	
	return false;
}

bool ServerSocket::RecvInfomation(std::string & _outString)
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
	// 待ち行列の作成。
	if (listen(this->acceptSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->acceptSocket, "listen()");
		return false;
	}
	return true;
}

// 解放
void ServerSocket::Release()
{
	closesocket(acceptSocket);
	clientVector.clear();
}