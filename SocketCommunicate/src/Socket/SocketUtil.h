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
		fprintf(stderr, "Error Code = %d", WSAGetLastError());	// �G���[�R�[�h�̕\��
		fprintf(stderr, _funcName.c_str(), _funcName);	// �G���[�֐��̕\��

		// socket()�ȊO�̃G���[�Ȃ烉�C�u���������
		if (_errorSocket != 0)
		{
			closesocket(_errorSocket);	// ���C�u�����̏I������
			return;
		}
		WSACleanup();
	}
};

#endif