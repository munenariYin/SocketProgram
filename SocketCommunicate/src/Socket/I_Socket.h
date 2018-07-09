#ifndef __I_Socket_h__
#define __I_Socket_h__

#define _CRT_SECURE_NO_WARNINGS		// セキュア警告抑止

#pragma comment(lib, "wsock32.lib")	//WinSockインポートライブラリ

#include <winsock.h>
#include <string>


class I_Socket
{
public:
	I_Socket(){}
	virtual ~I_Socket() {}


	// ソケットの生成。 WSAStartup ～ socketまで
	bool virtual SetUp(int _addressFamily, int _type, int _protocol) = 0;
	// acceptやconnectでの接続
	bool virtual Connect() = 0;
	
	// sendで送信する
	bool virtual SendInfomation(std::string _type, std::string _values) = 0;
	// recvによる受信
	bool virtual RecvInfomation(std::string& _outString) = 0;

	// ソケットの解放
	void virtual Release() = 0;

	void virtual SetPortNumber(unsigned short _portNo) = 0;

protected:
	bool virtual CreateSocket(int _addressFamily, int _type, int _protocol) = 0;
};

#endif