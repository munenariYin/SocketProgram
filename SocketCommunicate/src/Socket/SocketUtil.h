#ifndef __SocketUtil_h__
#define __SocketUtil_h__

#include"winsock.h"
#include<string>


class SocketUtil
{
public:
	static void SockError(SOCKET _errorSocket, const std::string& _funcName)
	{
		fprintf(stderr, "Error Code = %d", WSAGetLastError());	// エラーコードの表示
		fprintf(stderr, _funcName.c_str(), _funcName);	// エラー関数の表示

		// socket()以外のエラーならライブラリを閉じる
		//if (_errorSocket != 0)
		//{
		//	closesocket(_errorSocket);	// ライブラリの終了処理
		//	return;
		//}
	}

	// 受けた文字列がアドレスなのか、名前なのかは分からない。
	static unsigned long TransHostAddr(const char* _hostInfo)
	{
		struct hostent *phe;
		unsigned long ipAddr = inet_addr(_hostInfo);
		if (ipAddr == INADDR_NONE)		// INADDR_NONEはアドレスではないことを示す。
		{
			phe = gethostbyname(_hostInfo);	// ホスト名として処理
		}
		else // ホストアドレスだった場合
		{
			phe = gethostbyaddr((char*)&ipAddr, 4, AF_INET);	// addressとして処理
		}
		if (phe == NULL)
		{
			return 0UL;
		}
		printf("HostName\t: %s\n", phe->h_name);						// 公式名
		for (int i = 0; phe->h_aliases[i] != NULL; i++)
		{
			printf("Aliase Name[%d]\t: %hu\n", i, phe->h_aliases[i]);	// 別名
		}
		printf("Address Type\t: %hu", phe->h_addrtype);					// アドレス型
		printf("Address Length\t %hu\n", phe->h_length);				// アドレス長
		for (int i = 0; phe->h_addr_list[i]; i++)
		{
			printf("IP address[%d]\t: %s\n", i, inet_ntoa(*(struct in_addr*)phe->h_addr_list[i]));
		}
		if (*phe->h_addr_list == NULL)
		{
			return 0UL;
		}
		return *(unsigned long*)*phe->h_addr_list;
	}
};

#endif