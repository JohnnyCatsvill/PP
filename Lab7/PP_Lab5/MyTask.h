#pragma once
#include <iostream>
#include <string>
#include "ITask.h"

class MyTask: public ITask
{
private:
	int _value;
	
public:
	MyTask(int value): 
		_value(value) 
	{

	}

    void Execute() override
    {
        std::cout << "Поток №" + std::to_string(_value) + " выполняет свою работу\n";
    }
};

