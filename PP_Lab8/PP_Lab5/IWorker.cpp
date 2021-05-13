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

bool IWorker::CreateTask(ITask* taskToRun)
{
	if (_end)
	{
		return false;
	}

	IWorkerStruct* dataToSend = new IWorkerStruct{ *taskToRun };
	HANDLE* newHandle = new HANDLE(CreateThread(NULL, 0, &ThreadProc, (LPVOID)(dataToSend), CREATE_SUSPENDED, NULL));

	_handles.push_back(newHandle);

	return true;
}

void IWorker::ResumeTasks()
{
	int openedThreads = 0;

	for (auto i = _handles.begin(); i != _handles.end(); i++)
	{
		if (_processingMode == MODE_ALL_AT_ONCE)
		{
			ResumeThread(*i[0]);
			openedThreads++;
		}
		else if (_processingMode == MODE_PARTIALLY)
		{
			if (openedThreads <= _maxThreads)
			{
				openedThreads++;
				ResumeThread(*i[0]);
			}
			else
			{
				auto waitForThisHandle = i;
				for (int j = 0; j < _maxThreads; j++) waitForThisHandle--;
				WaitForMultipleObjects(1, *waitForThisHandle, true, INFINITE);
				ResumeThread(*i[0]);

				//_handles.erase(waitForThisHandle);

			}
		}
	}

	for (int i = 0; i < openedThreads; i++)
	{
		auto waitForThisHandle = _handles.end();
		for (int j = 0; j <= i; j++) waitForThisHandle--;
		WaitForMultipleObjects(1, *waitForThisHandle, true, INFINITE);
	}

}

bool IWorker::IsBusy()
{
	return _handles.size() > 0;
}

