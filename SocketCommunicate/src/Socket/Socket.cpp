#include"winsock.h"
#include<string>

#include"Socket.h"
#include"SocketUtil.h"

// �\�P�b�g�̐����B�v���g�R���A
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

// �o�C���h
bool Socket::Bind(short addressFamily, unsigned long _inAddr, unsigned short _portNo)
{
	memset(&this->addr, 0, sizeof this->addr);	// �[���N���A
	this->addr.sin_family = addressFamily;				// �A�h���X�t�@�~��
	this->addr.sin_addr.s_addr = htonl(_inAddr);		// IP�A�h���X
	this->addr.sin_port = htons(_portNo);				// �|�[�g�ԍ�

	// �\�P�b�g�Ɋ֘A�t����
	if (bind(this->sock, (struct sockaddr*)& this->addr, sizeof this->addr) == SOCKET_ERROR)
	{
		SocketUtil::SockError(this->sock, "bind()");
		return false;
	}
	return true;
}

// �҂��s��̐���
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
	// �B�T�[�o�ɐڑ���v��
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
	// �ЂƂ܂�char*�ŕ�������󂯎�邵���Ȃ�
	char* recvString = 0;
	int recvLength = recv(this->sock, recvString, _recvSize, _flags);
	// �󂯎�����������string�̃f�[�^�ɒǉ�����B
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
	// �u���b�L���O�����0�ɁA�����łȂ����0�ȊO��
	if (_isBlocking) val = 0;
	else val = 1;
	ioctlsocket(this->sock, FIONBIO, &val);
}

void Socket::Release()
{
	closesocket(this->sock);
}
