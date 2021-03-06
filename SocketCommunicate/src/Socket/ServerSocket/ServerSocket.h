#ifndef __ServerSocket_h__
#define __ServerSorket_h__

#include "../I_Socket.h"
#include <vector>



class ServerSocket : public I_Socket
{
public:
	
	ServerSocket(){}
	virtual ~ServerSocket(){}
	virtual bool SetUp(int _addressFamily, int _type, int _protocol) override;
	virtual bool Connect() override;
	virtual bool SendInfomation(std::string _type, std::string _values) override;
	virtual bool RecvInfomation(std::string& _outString) override;

	virtual void Release() override;

	virtual void SetPortNumber(unsigned short _portNo) override
	{
		this->portNo = _portNo;
	}
protected:
	virtual bool CreateSocket(int _addressFamily, int _type, int _protocol) override;

private:
	bool BindSocket();
	bool ListenSocket();

	unsigned short portNo;
	// 受付用ソケット
	SOCKET acceptSocket;
	sockaddr_in serverAddr;

	// クライアント用配列
	std::vector<SOCKET> clientVector;
};

#endif