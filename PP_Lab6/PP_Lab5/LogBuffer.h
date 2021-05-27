#pragma once
#include "IntQueue.h"
#include <string>
#include <windows.h>
#include <fstream>

extern CRITICAL_SECTION CriticalSection;
class LogBuffer
{

private:
	const int MAX_LOG_SIZE = 10000;

	HANDLE& _reachedMaxEvent;
	IntQueue& _data;

public:
	
	LogBuffer(IntQueue& data, HANDLE& reachedMaxEvent):
		_data(data), _reachedMaxEvent(reachedMaxEvent)
	{
		
	}

	~LogBuffer()
	{
		
	}
	
	void Add(int logData)
	{
		EnterCriticalSection(&CriticalSection);
		_data.Push(logData);
		LeaveCriticalSection(&CriticalSection);

		if (_data.GetSize() > MAX_LOG_SIZE)
		{
			SetEvent(_reachedMaxEvent);
		}
	}
};

