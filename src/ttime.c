
#include "stdafx.h"

#include "ttime.h"

#ifdef __cplusplus
extern "C" {
#endif

static double current_time;

#ifdef _WINDOWS
static double s_tickInterval = 1.0;

void ttime_initialise()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	s_tickInterval = 1.0 / (double)freq.QuadPart;
	current_time = 0.0;
}

void ttime_compute_time()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	current_time = (double)count.QuadPart * s_tickInterval;
}
#endif

#ifdef _MACOSX

uint64_t					s_start;
mach_timebase_info_data_t 	s_timebase;

void ttime_initialise()
{
	mach_timebase_info(&s_timebase);
	s_start = mach_absolute_time();
	current_time = 0.0;
}

void ttime_compute_time()
{
	uint64_t elapsed = mach_absolute_time() - s_start;
	current_time = double(elapsed) * (s_timebase.numer / s_timebase.denom) / 1000000000.0;
}
#endif

#ifdef _LINUX
static timeval s_start;

void ttime_initialise()
{
	gettimeofday(&s_start, NULL);
	current_time = 0.0;
}

void ttime_compute_time()
{
	timeval now;
	gettimeofday(&now, NULL);

	current_time = double(now.tv_sec - s_start.tv_sec) + (double(now.tv_usec) - double(s_start.tv_usec)) / 1000000.0;
}
#endif

double ttime_get_time()
{
	ttime_compute_time();
	return current_time;
}

double ttime_fetch_time()
{
	return current_time;
}

//------------- TTimer ---------------//

TTimer *ttimer_new(void)
{
	TTimer *t = (TTimer *) malloc(sizeof(TTimer));
	ttimer_init(t);
	return t;
}

#ifdef __cplusplus
}
#endif
