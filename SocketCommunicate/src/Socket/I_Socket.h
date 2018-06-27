#ifndef __I_Socket_h__
#define __I_Socket_h__

#include <winsock.h>
#include <string>

using namespace std;

class I_Socket
{
public:
	virtual ~I_Socket() = 0;

	// ソケットの生成。 WSAStartup ～ socketまで
	bool virtual SetUp(unsigned short _majorVer, unsigned short _minorVer) = 0;

	// acceptやconnectでの接続
	bool virtual Connect() = 0;
	
	// sendで送信する
	bool virtual SendInfomation(string _type, string _values) = 0;
	bool virtual RecvInfomation(string& _outString) = 0;

protected:
	bool virtual CreateSocket(int _addressFamily, int _type, int _protocol) = 0;
};

#endif