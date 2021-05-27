#pragma once
#include "IntQueue.h"

extern CRITICAL_SECTION CriticalSection;

bool isClassClosing = false;

struct LogWriterStruct
{
	HANDLE& trigger;
	IntQueue& data;
	std::string fileName;
};

DWORD WINAPI LogSizeMonitoringThread(LPVOID packedData)
{
	LogWriterStruct* refinedData = static_cast<LogWriterStruct*>(packedData);
	
	HANDLE& trigger = refinedData->trigger;
	IntQueue& queue = refinedData->data;
	std::string fileName = refinedData->fileName;


	while (!isClassClosing or queue.GetSize() > 0)
	{
		
		WaitForSingleObject(trigger, INFINITE);

		EnterCriticalSection(&CriticalSection);
		std::ofstream out;
		out.open(fileName, std::ios::ate | std::ios::app);

		

		while (queue.GetSize() > 0)
		{
			out << queue.Pop() << "\n";
			
		}
		out.close();

		ResetEvent(trigger);

		LeaveCriticalSection(&CriticalSection);
	}
	ExitThread(0);
}

class LogFileWriter
{
private:
	HANDLE& _trigger;

public:
	LogFileWriter(IntQueue& data, std::string fileName, HANDLE& triggerEvent):
		_trigger(triggerEvent)
	{

		std::ofstream out;
		out.open(fileName);
		out.close();

		LPVOID dataToSend = new LogWriterStruct{ triggerEvent, data, fileName };

		HANDLE handle = CreateThread(NULL, 0, &LogSizeMonitoringThread, dataToSend, CREATE_SUSPENDED, NULL);
		ResumeThread(handle);
	}

	~LogFileWriter()
	{
	}

	void Close()
	{
		SetEvent(_trigger);
		isClassClosing = true;
	}
};

