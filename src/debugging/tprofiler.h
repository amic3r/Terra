
#ifndef __included_terra_profiler_h
#define __included_terra_profiler_h

#ifdef _DEBUG

typedef struct {
	double currentTotalTime;
	int currentNumCalls;

	double lastTotalTime;
	int lastNumCalls;

	double historyTotalTime;
	int historyNumCalls;
	int numAdvances;

	double shortest;
	double longest;
	
	double timerStart;
	char *name;
} TProfile;

void TProfileStart(TProfile *p);
void TProfileStop(TProfile *p);

//--- Profiler ---------------------------//

void TProfilerInitialise(void);
void TProfilerDestroy(void);

TProfile *TProfilerProfile(const char *name);

void TProfilerAdvance(void);
void TProfilerResetHistory(void);
#endif

#endif
