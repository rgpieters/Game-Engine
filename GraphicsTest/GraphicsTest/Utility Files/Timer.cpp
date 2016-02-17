#include "Timer.h"

Timer::Timer()
{

}

void Timer::Start()
{
	m_ElapsedTime = 0.0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);

	m_ticksPerMs = (float)(m_Frequency / 10);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTick);
}

void Timer::Update()
{
	LONGLONG stopTick;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&stopTick);

	timeDifference = (float)(stopTick - m_StartTick);
	m_ElapsedTime = timeDifference / m_ticksPerMs;

	m_StartTick = stopTick;
}

float Timer::GetElapsedTime()
{
	return m_ElapsedTime;
}