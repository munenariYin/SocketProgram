#ifndef __SocketUtil_h__
#define __SocketUtil_h__

#include"winsock.h"
#include<string>

using namespace std;

class SocketUtil
{
public:
	static void SockError(SOCKET _errorSocket, const string& _funcName)
	{
		fprintf(stderr, "Error Code = %d", WSAGetLastError());	// エラーコードの表示
		fprintf(stderr, _funcName.c_str(), _funcName);	// エラー関数の表示

		// socket()以外のエラーならライブラリを閉じる
		if (_errorSocket != 0)
		{
			closesocket(_errorSocket);	// ライブラリの終了処理
			return;
		}
		WSACleanup();
	}
};

#endif