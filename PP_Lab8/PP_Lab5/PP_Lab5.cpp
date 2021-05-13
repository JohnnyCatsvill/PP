#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include "ITask.h"
#include "MyTask.h"
#include "IWorker.h"
#include <filesystem>

CRITICAL_SECTION CriticalSection;

int main(int argc, char* argv[])
{

    int processingMode = atoi(argv[1]);
    int amountOfBlocksPerImage = atoi(argv[2]);
    std::string inputDir = argv[3];
    std::string outputDir = argv[4];
    int maxThreadAmount = atoi(argv[5]);

    IWorker worker(processingMode, maxThreadAmount);
    
    using directory_iterator = std::filesystem::directory_iterator;

    std::vector<bitmap*> allOuts;
    
    for (const auto& dirEntry : directory_iterator(inputDir))
    {
        std::cout << (dirEntry.path().string()).c_str() << std::endl;
        
        bitmap* out = new bitmap((outputDir + "\\" + dirEntry.path().filename().string()).c_str());
        out = new bitmap((dirEntry.path().string()).c_str());
        out->save((outputDir + "\\" + dirEntry.path().filename().string()).c_str());
        allOuts.push_back(out);

        for (int i = 0; i < amountOfBlocksPerImage; i++)
        {
            ITask* my_class = new MyTask((dirEntry.path().string()).c_str(), (outputDir + "\\" + dirEntry.path().filename().string()).c_str(), i+1, amountOfBlocksPerImage, out);
            worker.CreateTask(my_class);
        }
    }

    std::cout << "\n";

    worker.ResumeTasks();

    return 0;
}
