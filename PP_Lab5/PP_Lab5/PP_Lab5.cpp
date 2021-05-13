#include <windows.h>
#include <string>
#include <iostream>
#include <ctime>
#include "IMyClass.h"
#include "MyClass.h"
#include "ThreadClass.h"
#include <random>

const int NUM_OF_THREADS = 2;
const int NUM_OF_PROCESSORS = 2;

const int SHARED_NUMBER = 1;

CRITICAL_SECTION CriticalSection;

int main(int argc, char* argv[])
{
    int shared_number = SHARED_NUMBER;
    IMyClass* my_class = new MyClass(&shared_number);
    
    ThreadClass thread_class(NUM_OF_THREADS, NUM_OF_PROCESSORS);
    thread_class.CreateSuspendedTasks(my_class);
    thread_class.RunTasks();

    return 0;
}
