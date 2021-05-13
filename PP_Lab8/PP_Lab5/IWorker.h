#pragma once
#include <windows.h>
#include <iostream>
#include "ITask.h"
#include <vector>

extern CRITICAL_SECTION CriticalSection;

const int MODE_ALL_AT_ONCE = 0;
const int MODE_PARTIALLY = 1;

class IWorker
{
private:
	std::vector<HANDLE*> _handles;
	bool _bisy = false;
	bool _end = false;

	int _processingMode;
	int _maxThreads;

public:
	IWorker(int processingMode, int maxAmountOfThreads = 1):
		_processingMode(processingMode), _maxThreads(maxAmountOfThreads)
	{
		InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
	}

	~IWorker()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	bool CreateTask(ITask* taskToRun);
	void ResumeTasks();

	bool IsBusy();
};

