
#include "stdafx.h"

#include "ttime.h"
#include "talloc.h"

#ifdef __cplusplus
extern "C" {
#endif

static double tCurrentTime;

#ifdef _WINDOWS
static double tTickInterval = 1.0;

#define _WIN32_WINDOWS 0x0500      // for IsDebuggerPresent
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void TTimeInitialise()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	tTickInterval = 1.0 / (double)freq.QuadPart;
	tCurrentTime = 0.0;
}

void TTimeComputeTime()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	tCurrentTime = (double)count.QuadPart * tTickInterval;
}
#endif

#ifdef _MACOSX

static uint64_t						tStart;
static mach_timebase_info_data_t 	tTimebase;

void TTimeInitialise()
{
	mach_timebase_info(&tTimebase);
	tStart = mach_absolute_time();
	tCurrentTime = 0.0;
}

void TTimeComputeTime()
{
	uint64_t elapsed = mach_absolute_time() - tStart;
	tCurrentTime = (double)elapsed * (tTimebase.numer / tTimebase.denom) / 1000000000.0;
}
#endif

#ifdef _LINUX
#include <sys/time.h>
static struct timeval tStart;

void TTimeInitialise()
{
	gettimeofday(&tStart, NULL);
	tCurrentTime = 0.0;
}

void TTimeComputeTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);

	tCurrentTime = (double)(now.tv_sec - tStart.tv_sec) + ((double)now.tv_usec - (double)tStart.tv_usec) / 1000000.0;
}
#endif

double TTimeGetTime()
{
	TTimeComputeTime();
	return tCurrentTime;
}

double TTimeFetchTime()
{
	return tCurrentTime;
}

void TTimeSleep(size_t msec) {
#ifdef _WINDOWS
	Sleep(msec);
#else
	usleep(msec);
#endif
}

//------------- TTimer ---------------//

TTimer *ttimer_new(void)
{
	TTimer *t = (TTimer *) TAlloc(sizeof(TTimer));
	if(!t) return 0;

	TTimerInit(t);
	return t;
}

#ifdef __cplusplus
}
#endif
