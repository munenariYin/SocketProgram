#ifndef __I_Socket_h__
#define __I_Socket_h__

#define _CRT_SECURE_NO_WARNINGS		// �Z�L���A�x���}�~

#pragma comment(lib, "wsock32.lib")	//WinSock�C���|�[�g���C�u����

#include <winsock.h>
#include <string>


class I_Socket
{
public:
	I_Socket(){}
	virtual ~I_Socket() {}


	// �\�P�b�g�̐����B WSAStartup �` socket�܂�
	bool virtual SetUp(int _addressFamily, int _type, int _protocol) = 0;
	// accept��connect�ł̐ڑ�
	bool virtual Connect() = 0;
	
	// send�ő��M����
	bool virtual SendInfomation(std::string _type, std::string _values) = 0;
	// recv�ɂ���M
	bool virtual RecvInfomation(std::string& _outString) = 0;

	// �\�P�b�g�̉��
	void virtual Release() = 0;

	void virtual SetPortNumber(unsigned short _portNo) = 0;

protected:
	bool virtual CreateSocket(int _addressFamily, int _type, int _protocol) = 0;
};

#endif