#ifndef __Socket_h__
#define __Socket_h__


enum class SockErrorType
{
	COMPLETE,
	ERROR_1
};

// ================================================
// SOCKET�̃��b�v�N���X
// ================================================

class Socket
{
public:
	Socket(){}
	~Socket()
	{
		//closesocket(this->sock);
	}

	// /////////////////////////////////////////////
	// =============================================
	// �\�P�b�g�̋@�\
	// =============================================
	// /////////////////////////////////////////////

	// socket()�B�v���g�R���t�@�~��,�\�P�b�g�^�C�v,�v���g�R�����w�肷��B
	// TCP��UDP���́A���̎��_�Ŏw�肷��B
	bool CreateSocket(int _protocolFamily, int _type, int _protocol);
	// bind()�B�A�h���X�t�@�~��, �ڑ��ł���A�h���X�̃}�X�N, �|�[�g�ԍ����w�肷��
	bool Bind(short addressFamily, unsigned long _inAddr, unsigned short _portNo);
	// listen()�B�o�b�N���O(�҂��s��)�̍쐬
	bool Listen(int _backLog);
	// connect()�B�ڑ���҂B
	bool Connect();
	// accept()�B�����̃\�P�b�g�Ƃ̐ڑ����󂯕t����B
	bool Accept(Socket& _targetSock);
	// recv()�B������̎󂯎��B
	int Receive(std::string& _sendData, int _sendSize, int _flags);
	// send()�B������̑��M�B
	int Send(std::string& _sendData, int _sendSize, int _flags);

	void SetBlocking(bool _isBlocking);

	void Release();

	// ==================================
	// Getter
	// ==================================
	sockaddr_in& GetAddr()
	{
		return this->addr;
	}

	SOCKET GetSocket()
	{
		return this->sock;
	}

	// ==================================
	// Setter
	// ==================================
	void SetSocket(SOCKET _socket)
	{
		this->sock = _socket;
	}
	void SetAddr(sockaddr_in& _addr)
	{
		this->addr = _addr;
	}

public:
	static const int MSG_MAX = 1024;

private:
	SOCKET sock;
	sockaddr_in addr;
	bool isBlocking = false;
	bool isAccepting = false;
	bool isConnecting = false;
};

#endif