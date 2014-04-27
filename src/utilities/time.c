

#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

static double current_time;

#ifdef _WINDOWS
static double s_tickInterval = 1.0;

void InitialiseHighResTime()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	s_tickInterval = 1.0 / (double)freq.QuadPart;
	current_time = 0.0;
}

void ComputeHighResTime()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	current_time = (double)count.QuadPart * s_tickInterval;
}
#endif

#ifdef TARGET_OS_MACOSX

uint64_t					s_start;
mach_timebase_info_data_t 	s_timebase;

void InitialiseHighResTime()
{
	mach_timebase_info(&s_timebase);
	s_start = mach_absolute_time();
	current_time = 0.0;
}

void ComputeHighResTime()
{
	uint64_t elapsed = mach_absolute_time() - s_start;
	current_time = double(elapsed) * (s_timebase.numer / s_timebase.denom) / 1000000000.0;
}
#endif

#ifdef TARGET_OS_LINUX
static timeval s_start;

void InitialiseHighResTime()
{
	gettimeofday(&s_start, NULL);
	current_time = 0.0;
}

void ComputeHighResTime()
{
	timeval now;
	gettimeofday(&now, NULL);

	current_time = double(now.tv_sec - s_start.tv_sec) + (double(now.tv_usec) - double(s_start.tv_usec)) / 1000000.0;
}
#endif

double GetHighResTime()
{
	ComputeHighResTime();
	return current_time;
}

double fetchHighResTime()
{
	return current_time;
}

#ifdef __cplusplus
}
#endif
