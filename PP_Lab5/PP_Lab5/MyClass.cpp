#include "MyClass.h"

void MyClass::Run()
{
    srand(time(0));
    _delta = rand() % 10 + 1;

    _value += _delta;                             
    _value_adress = &_value;                       

    if (*_value_adress != _value)
    {
        std::cout << "Error: Numbers are not same\n";
    }
    else
    {
        std::cout << "Shared Number - " + std::to_string(*_value_adress) + " *** Local Value - " + std::to_string(_value) + "\n";
    }
}
