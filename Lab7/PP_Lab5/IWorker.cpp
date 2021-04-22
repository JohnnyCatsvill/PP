#include "IWorker.h"

extern CRITICAL_SECTION CriticalSection;

struct IWorkerStruct
{
	ITask& task;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	IWorkerStruct* refinedData = (IWorkerStruct*)lpParam;
	ITask& task = refinedData->task;

	EnterCriticalSection(&CriticalSection);
	task.Execute();
	LeaveCriticalSection(&CriticalSection);
	ExitThread(0);
}

bool IWorker::ExecuteTask(ITask* taskToRun)
{
	if (_end)
	{
		return false;
	}

	IWorkerStruct* dataToSend = new IWorkerStruct{ *taskToRun };
	HANDLE newHandle = CreateThread(NULL, 0, &ThreadProc, (LPVOID)(dataToSend), CREATE_SUSPENDED, NULL);
	
	_handles.push_back(newHandle);
	if (_handles.size() == 1)
	{
		ResumeThread(_handles.front());
	}

	WaitForSingleObject(_handles.back(), INFINITE);
	if (_handles.size() > 0 and !_end)
	{
		ResumeThread(_handles.front());
	}
	_handles.erase(_handles.begin());

	if (_end)
	{
		return false;
	}

	return true;
}

bool IWorker::IsBusy()
{
	return _handles.size() > 0;
}

