#pragma once
#include <iostream>
#include <string>
#include "IMyClass.h"

class MyClass: public IMyClass
{
private:
	int* _value_adress;
	int _value;
	int _delta;

public:
	MyClass(int* value): 
		_value_adress(value), _value(*value) 
	{

	}

	void Run() override;
};

