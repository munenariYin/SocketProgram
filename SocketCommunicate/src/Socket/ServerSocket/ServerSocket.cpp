
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
		// �F���b�Z�[�W��M
		len = recv(talkSocket, msg, MSG_SIZE, 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(talkSocket, "recv()");
			break;
		}
		msg[len] = '\0';
		printf("��M:%s\n", msg);
		// �E���b�Z�[�W���M
		len = send(talkSocket, msg, (int)strlen(msg), 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(talkSocket, "send()");
			break;
		}
		printf("���M:%s\n", msg);
	} while (strncmp(msg, "bye", 3) != 0);	// ��b�I������

											// �G��b�p�\�P�b�g�����
	closesocket(talkSocket);
	_endthread();	//�X���b�h�̏I��

	return;
}

bool ServerSocket::Connect()
{

	//// �}���`�X���b�h
	//while (true)
	//{
	//	struct sockaddr_in clientAddr;
	//	int len = sizeof clientAddr;

	//	// �D�ڑ��v����t
	//	SOCKET talkSocket;


	//	// �D�ڑ��v����t
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
	//			// �F���b�Z�[�W��M
	//			len = recv(talkSocket, msg, MSG_SIZE, 0);
	//			if (len == SOCKET_ERROR)
	//			{
	//				SocketUtil::SockError(talkSocket, "recv()");
	//			}
	//			msg[len] = '\0';
	//			printf("��M:%s\n", msg);
	//			// �E���b�Z�[�W���M
	//			len = send(talkSocket, msg, (int)strlen(msg), 0);
	//			if (len == SOCKET_ERROR)
	//			{
	//				SocketUtil::SockError(talkSocket, "send()");
	//			}
	//			printf("���M:%s\n", msg);
	//		} while (strncmp(msg, "bye", 3) != 0);	// ��b�I������

	//		// �G��b�p�\�P�b�g�����
	//		closesocket(talkSocket);
	//		return;
	//	};
	//	// �X���b�h�̐؂藣��
	//	communicateThread.detach();

	//	//uintptr_t hThread = _beginthread(SockThread, 0, (void*)talkSocket);

	//	//if (hThread == -1)
	//	//{
	//	//	printf("thread creation error. \n");
	//	//	closesocket(talkSocket);
	//	//	break;
	//	//}

	//	printf("Client	IP	: %s\n", inet_ntoa(clientAddr.sin_addr));	// IP�A�h���X,network to ASCII
	//	printf("Client	Port: %hu\n", ntohs(clientAddr.sin_port));		// �|�[�g�ԍ�,network to host short
	//																	// �E���b�Z�[�W���M
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
		// fe_set�̏���������
		FD_ZERO(&checkFD);	// ���t���[���[���ɃN���A����
		// ��t�p�\�P�b�g�̃Z�b�g
		FD_SET(this->acceptSocket, &checkFD);
		// �N���C�A���g�p�\�P�b�g�̃Z�b�g
		for (auto clientSocket : this->clientVector)
		{
			FD_SET(clientSocket, &checkFD);
		}

		// �\�P�b�g�ɒʐM�����Ă��邩
		int callSocketCnt = select(
			FD_SETSIZE,	// UNIX�Ƃ̌݊����ׁ̈Bfd_set�̒���socket��SETSIZE�������m�F����
			&checkFD,
			NULL,
			NULL,
			&timeOut);
		// �^�C���A�E�g��

		if (callSocketCnt == 0)
		{
			timeOutCnt++;
			fprintf(stderr, "select timeout.\n");
			// ������^�C���A�E�g����������ΏI��
			if (timeOutCnt > TIMEOUT_CNT_MAX) return false;
			// ������x�Aselect�𑖂点��
			continue;
		}
		timeOutCnt = 0;

		// accept�p�\�P�b�g�ɗv�����m�F�ł����ꍇ�A�N���C�A���g�p�\�P�b�g������
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
			printf("���M:%s\n", "hello");
		}

		// �v�������Ă���N���C�A���g�Ƃ������΂���B
		for (int i = this->clientVector.size() - 1; i >= 0; i--)
		{
			if (FD_ISSET(this->clientVector[i], &checkFD))
			{
				const int MSG_SIZE = 1024;
				char msg[MSG_SIZE + 1];
				int len;

				// �F���b�Z�[�W��M
				len = recv(this->clientVector[i], msg, MSG_SIZE, 0);
				if (len == SOCKET_ERROR)
				{
					SocketUtil::SockError(this->clientVector[i], "recv()");
				}
				msg[len] = '\0';
				printf("��M:%s\n", msg);
				// �E���b�Z�[�W���M
				len = send(this->clientVector[i], msg, (int)strlen(msg), 0);
				if (len == SOCKET_ERROR)
				{
					SocketUtil::SockError(this->clientVector[i], "send()");
				}
				printf("���M:%s\n", msg);

				if (strncmp(msg, "bye", 3) == 0)
				{
					this->clientVector.erase(clientVector.begin() + i);
				}
			}
		}
		//// �����\�P�b�g�Ȃ�I��
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

		// �}���`�X���b�h���� //////////////////////////////////
		// "bye"������ꂽ��I��

		//std::thread communicateThread([](SOCKET _clientSocket)
		//{
		//	const int MSG_SIZE = 1024;
		//	char msg[MSG_SIZE + 1];
		//	int len;

		//	SOCKET talkSocket;
		//	talkSocket = (SOCKET)_clientSocket;

		//	do
		//	{
		//		// �F���b�Z�[�W��M
		//		len = recv(talkSocket, msg, MSG_SIZE, 0);
		//		if (len == SOCKET_ERROR)
		//		{
		//			SocketUtil::SockError(talkSocket, "recv()");
		//		}
		//		msg[len] = '\0';
		//		printf("��M:%s\n", msg);
		//		// �E���b�Z�[�W���M
		//		len = send(talkSocket, msg, (int)strlen(msg), 0);
		//		if (len == SOCKET_ERROR)
		//		{
		//			SocketUtil::SockError(talkSocket, "send()");
		//		}
		//		printf("���M:%s\n", msg);
		//	} while (strncmp(msg, "bye", 3) != 0);	// ��b�I������

		//	// �G��b�p�\�P�b�g�����
		//	closesocket(talkSocket);
		//	return;
		//});
		//// �X���b�h�̐؂藣��
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


// ���ڑ��̃\�P�b�g�ɑ΂��čs��
bool ServerSocket::BindSocket()
{
	memset(&this->serverAddr, 0, sizeof this->serverAddr);	// �[���N���A
	this->serverAddr.sin_family = AF_INET;					// �A�h���X�t�@�~��
	this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP�A�h���X
	this->serverAddr.sin_port = htons(this->portNo);		// �|�[�g�ԍ�

	// �\�P�b�g�Ɋ֘A�t����
	if (bind(this->acceptSocket, (struct sockaddr*)& this->serverAddr, sizeof this->serverAddr) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->acceptSocket, "bind()");
		return false;
	}

	return true;
}

bool ServerSocket::ListenSocket()
{
	// �҂��s��̍쐬�B
	if (listen(this->acceptSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->acceptSocket, "listen()");
		return false;
	}
	return true;
}

// ���
void ServerSocket::Release()
{
	closesocket(acceptSocket);
	clientVector.clear();
}