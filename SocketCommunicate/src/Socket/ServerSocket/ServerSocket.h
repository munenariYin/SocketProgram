#ifndef __ServerSocket_h__
#define __ServerSorket_h__

#include "../I_Socket.h"

class ServerSocket : public I_Socket
{
public:
	
	ServerSocket(){}
	virtual ~ServerSocket(){}
	virtual bool SetUp(unsigned short _majorVer, unsigned short _minorVer) override;
	virtual bool Connect() override;
	virtual bool SendInfomation(string _type, string _values) override;
	virtual bool RecvInfomation(string& _outString) override;
protected:
	virtual bool CreateSocket(int _addressFamily, int _type, int _protocol) override;

private:
	bool BindSocket();
	bool ListenSocket();

	unsigned short portNo;
	WSADATA wsaData;
	SOCKET acceptSocket;
	sockaddr_in serverAddr;
};

#endif