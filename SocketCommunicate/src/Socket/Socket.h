#ifndef __Socket_h__
#define __Socket_h__


enum class SockErrorType
{
	COMPLETE,
	ERROR_1
};

// ================================================
// SOCKETのラップクラス
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
	// ソケットの機能
	// =============================================
	// /////////////////////////////////////////////

	// socket()。プロトコルファミリ,ソケットタイプ,プロトコルを指定する。
	// TCPかUDPかは、この時点で指定する。
	bool CreateSocket(int _protocolFamily, int _type, int _protocol);
	// bind()。アドレスファミリ, 接続できるアドレスのマスク, ポート番号を指定する
	bool Bind(short addressFamily, unsigned long _inAddr, unsigned short _portNo);
	// listen()。バックログ(待ち行列)の作成
	bool Listen(int _backLog);
	// connect()。接続を待つ。
	bool Connect();
	// accept()。引数のソケットとの接続を受け付ける。
	bool Accept(Socket& _targetSock);
	// recv()。文字列の受け取り。
	int Receive(std::string& _sendData, int _sendSize, int _flags);
	// send()。文字列の送信。
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