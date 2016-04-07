#include "stdafx.h"

#include "Timer.h"

namespace SDK
{
	static float inv_freq = 1.f;

	__int64 get_time()
	{
		__int64 t;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&t));
		return t;
	}

	float Interval(__int64 i_begin, __int64 i_end)
	{
		if (i_begin <= i_end)
			return float(i_end - i_begin) * inv_freq;
		else
			return -float(i_begin - i_end) * inv_freq;
	}

	void Timer::Sync()
	{
		__int64 f;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&f));
		inv_freq = 1.0f / static_cast<float>(f);
	}

	void Timer::Start()
	{
		m_start_time = get_time();
		m_idle_ticks = 0;
	}
	void Timer::Pause()
	{
		m_idle_start_time = get_time();
	}

	void Timer::Resume()
	{
		__int64 t = get_time();
		m_idle_ticks += (t - m_idle_start_time);
	}

	float Timer::GetElapsedTime() const
	{
		float result = Interval(m_start_time + m_idle_ticks, get_time());
		if (result < 0 || result == 0.f)
			return 0.001f;
		return result;
	}

} // SDK