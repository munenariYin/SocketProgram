#ifndef __CommunicateThread_h__
#define __CommunicateThread_h__

// スレッドの終了を判断できるようにしたもの
class CommunicateThread
{
public:
	CommunicateThread();
	void BeginThread();
//	std::thread thread;
	bool isEndThread = false;
};

#endif