#include <windows.h>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

UINT8 start_time;
int times[320];

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    int param = (int)lpParam;

    int just_num;
    for (int n = 0; n < 20; n++)
    {
        for (int i = 0; i < 1000000; i++)
        {
            just_num = 1 / sqrt(i + 1);
        }

        times[n+param*20] = clock() - start_time;
    }
    ExitThread(0);
}


int main(int argc, char* argv[])
{
    const int count_threads = 16;
    HANDLE* handles = new HANDLE[count_threads];

    int rnd;

    std::cin >> rnd;
    
    start_time = clock();
    for (int i = 0; i < count_threads; i++) {
        handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)i, CREATE_SUSPENDED, NULL);
    }
    // запуск двух потоков
    for (int i = 0; i < count_threads; i++) {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы двух потоков
    WaitForMultipleObjects(count_threads, handles, true, INFINITE);

    for (int i = 0; i < 320; i++)
    {
        std::cout << times[i] << "\n";
    }

    return 0;
}
