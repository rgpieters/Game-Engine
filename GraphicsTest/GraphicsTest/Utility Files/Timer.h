#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer
{
private:
	LONGLONG m_StartTick;
	LONGLONG m_Frequency;
	float	 m_ticksPerMs;

	float m_ElapsedTime;

public:
	Timer();
	void Start();
	void Update();

	float GetElapsedTime();
};

#endif