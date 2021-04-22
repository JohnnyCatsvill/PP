#pragma once
#include <windows.h>
#include <iostream>
#include "ITask.h"
#include <vector>

extern CRITICAL_SECTION CriticalSection;

class IWorker
{
private:
	std::vector<HANDLE> _handles;
	bool _bisy = false;
	bool _end = false;

public:
	IWorker()
	{
		InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
	}

	~IWorker()
	{
		_end = true;
		if (!_handles.empty())
		{
			WaitForSingleObject(_handles.front(), INFINITE);
			for (auto i = _handles.begin(); i != _handles.end(); i++)
			{
				TerminateThread(*i, 1);
			}
		}
		DeleteCriticalSection(&CriticalSection);
	}

	bool ExecuteTask(ITask* taskToRun);
	bool IsBusy();
};

