#pragma once
#include <windows.h>
#include <iostream>
#include "IMyClass.h"

extern CRITICAL_SECTION CriticalSection;

class ThreadClass
{
private:
	unsigned int _num_of_theads;
	unsigned int _num_of_processors;
	unsigned int _limit_of_parallel_threads;

	DWORD _processor_affinity_mask = 0x0;
	HANDLE* _handles;

public:
	ThreadClass(unsigned int num_of_theads, unsigned int num_of_processors):
		_num_of_theads(num_of_theads), _num_of_processors(num_of_processors), _limit_of_parallel_threads(num_of_theads)
	{
		InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);

		for (int i = 0; i < num_of_processors; i++)
		{
			_processor_affinity_mask += pow(2, i);
		}

		_handles = new HANDLE[num_of_theads];
	}

	~ThreadClass()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	void SetLimitOfParallelThreads(unsigned int limit);
	void CreateSuspendedTasks(IMyClass* class_to_run);
	void RunTasks();
};

