#ifndef __CommunicateThread_h__
#define __CommunicateThread_h__

// �X���b�h�̏I���𔻒f�ł���悤�ɂ�������
class CommunicateThread
{
public:
	CommunicateThread();
	void BeginThread();
//	std::thread thread;
	bool isEndThread = false;
};

#endif