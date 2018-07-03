#ifndef __ClientSocket_h__
#define __ClientSocket_h__

#include"../I_Socket.h"
class ClientSocket : public I_Socket
{
public:
	ClientSocket(){}
	virtual ~ClientSocket(){}
	virtual bool SetUp(int _addressFamily, int _type, int _protocol) override;
	virtual bool Connect() override;
	virtual bool SendInfomation(std::string _type, std::string _values) override;
	virtual bool RecvInfomation(std::string & _outString) override;
	virtual void Release() override;

	virtual void SetPortNumber(unsigned short _portNo) override
	{
		this->portNo = _portNo;
	}
protected:
	virtual bool CreateSocket(int _addressFamily, int _type, int _protocol) override;

private:
	unsigned short portNo;
	WSADATA wsaData;
	SOCKET connectSocket;
	sockaddr_in serverAddr;

};

#endif