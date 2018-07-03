#ifndef __SocketUtil_h__
#define __SocketUtil_h__

#include"winsock.h"
#include<string>


class SocketUtil
{
public:
	static void SockError(SOCKET _errorSocket, const std::string& _funcName)
	{
		fprintf(stderr, "Error Code = %d", WSAGetLastError());	// �G���[�R�[�h�̕\��
		fprintf(stderr, _funcName.c_str(), _funcName);	// �G���[�֐��̕\��

		// socket()�ȊO�̃G���[�Ȃ烉�C�u���������
		//if (_errorSocket != 0)
		//{
		//	closesocket(_errorSocket);	// ���C�u�����̏I������
		//	return;
		//}
	}

	// �󂯂������񂪃A�h���X�Ȃ̂��A���O�Ȃ̂��͕�����Ȃ��B
	static unsigned long TransHostAddr(const char* _hostInfo)
	{
		struct hostent *phe;
		unsigned long ipAddr = inet_addr(_hostInfo);
		if (ipAddr == INADDR_NONE)		// INADDR_NONE�̓A�h���X�ł͂Ȃ����Ƃ������B
		{
			phe = gethostbyname(_hostInfo);	// �z�X�g���Ƃ��ď���
		}
		else // �z�X�g�A�h���X�������ꍇ
		{
			phe = gethostbyaddr((char*)&ipAddr, 4, AF_INET);	// address�Ƃ��ď���
		}
		if (phe == NULL)
		{
			return 0UL;
		}
		printf("HostName\t: %s\n", phe->h_name);						// ������
		for (int i = 0; phe->h_aliases[i] != NULL; i++)
		{
			printf("Aliase Name[%d]\t: %hu\n", i, phe->h_aliases[i]);	// �ʖ�
		}
		printf("Address Type\t: %hu", phe->h_addrtype);					// �A�h���X�^
		printf("Address Length\t %hu\n", phe->h_length);				// �A�h���X��
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