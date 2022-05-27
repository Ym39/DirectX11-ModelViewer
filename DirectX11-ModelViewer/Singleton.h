#pragma once
#include <stdio.h>
#include <assert.h>

template<typename T>
class Singletone
{
public:
	static T& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new T;
		}

		return *mInstance;
	}

private:
	static T* mInstance;
};

template<typename T>
T* Singletone<T>::mInstance = nullptr;
