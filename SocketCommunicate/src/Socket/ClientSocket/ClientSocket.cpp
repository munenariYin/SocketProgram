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
	// �B�T�[�o�ɐڑ���v��
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
		// �C���b�Z�[�W��M
		len = recv(connectSocket, msg, MSG_SIZE, 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(connectSocket, "recv()");
			break;
		}
		msg[len] = '\0';	// ������I�[����
		printf("��M:�@%s\n", msg);	// ��M���b�Z�[�W�̕\��
		if (strncmp(msg, "bye", 3) == 0) break; // ��b�I������

		printf("����: ");
		scanf("%s", msg);	// ���M���b�Z�[�W����

							// �D���b�Z�[�W���M
		len = send(connectSocket, msg, (int)strlen(msg), 0);
		if (len == SOCKET_ERROR)
		{
			SocketUtil::SockError(connectSocket, "send()");
			break;
		}
		printf("���M: %s\n", msg);	// ���M���b�Z�[�W�\��
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
	scanf("%s", host);		// �ڑ��������T�[�o��IP�A�h���X�����

	memset(&this->serverAddr, 0, sizeof this->serverAddr);	// �[���N���A
	this->serverAddr.sin_family = AF_INET;				// �A�h���X�t�@�~���[
	this->serverAddr.sin_addr.s_addr = SocketUtil::TransHostAddr(host);	// �T�[�o��IP�A�h���X
	this->serverAddr.sin_port = htons(this->portNo);			// �T�[�o�̃|�[�g�ԍ�
	return true;
}
