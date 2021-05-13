#include "ThreadClass.h"

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	IMyClass* my_class = (IMyClass*)lpParam;
	//EnterCriticalSection(&CriticalSection);
	my_class->Run();
	//LeaveCriticalSection(&CriticalSection);
	ExitThread(0);
}

void ThreadClass::RunTasks()
{
	for (int i = 0; i < _limit_of_parallel_threads; i++)
	{
		ResumeThread(_handles[i]);
	}

	for (int i = _limit_of_parallel_threads; i < _num_of_theads; i++)
	{
		WaitForMultipleObjects(1, _handles, true, INFINITE);
		ResumeThread(_handles[i]);
	}

	WaitForMultipleObjects(_limit_of_parallel_threads, _handles, true, INFINITE);
}

void ThreadClass::CreateSuspendedTasks(IMyClass* class_to_run)
{
	for (int i = 0; i < _num_of_theads; i++) {
		_handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)(class_to_run), CREATE_SUSPENDED, NULL);
		SetProcessAffinityMask(GetCurrentProcess(), _processor_affinity_mask);
	}
}

void ThreadClass::SetLimitOfParallelThreads(unsigned int limit)
{
	_limit_of_parallel_threads = limit;
}
