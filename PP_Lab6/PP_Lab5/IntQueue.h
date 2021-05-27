#pragma once

class IntQueue
{
private:
	struct IntQueueStruct
	{
		int data;
		IntQueueStruct* next;
	};
	
	IntQueueStruct* _end = new IntQueueStruct{ -2, nullptr };
	IntQueueStruct* _begin = new IntQueueStruct{ -1, _end };

	unsigned int _length = 0;

public:

	IntQueue() { }

	~IntQueue() 
	{
		delete(_end);
		delete(_begin);
	}

	void Push(int data)
	{
		IntQueueStruct* newObj = new IntQueueStruct{-2, nullptr};
		_end->data = data;
		_end->next = newObj;
		_end = newObj;
		_length++;
	}

	int Pop()
	{
		int out = _begin->next->data;

		IntQueueStruct* nextNextObj = _begin->next->next;
		delete(_begin->next);
		_begin->next = nextNextObj;

		_length--;
		return out;
	}

	int GetSize()
	{
		return _length;
	}
};

