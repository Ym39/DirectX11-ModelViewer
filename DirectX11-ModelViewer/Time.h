#pragma once
#include<Windows.h>

class Time
{
public:
	Time() = default;
	Time(const Time&) = default;
	~Time() = default;

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 mFrequency;
	float mTickPerMs;
	INT64 mStartTime;
	float mFrameTime;
};

