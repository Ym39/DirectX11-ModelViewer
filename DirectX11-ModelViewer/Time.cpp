#include "Time.h"

bool Time::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&mFrequency);
	if (mFrequency == 0)
		return false;

	mTickPerMs = (float)(mFrequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);

	return true;
}

void Time::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeDifference = (float)(currentTime - mStartTime);
	mFrameTime = timeDifference / mTickPerMs;
	mStartTime = currentTime;

	return;
}

float Time::GetTime()
{
	return mFrameTime;
}
