
#ifndef __included_terra_time_h
#define __included_terra_time_h

//------------- TTime ----------------//

void TTimeInitialise();
double TTimeGetTime();

void TTimeComputeTime();
double TTimeFetchTime();

void TTimeSleep(size_t msec);

//------------- TTimer ---------------//

typedef struct TTimer {
	double start, stop;
	double accumulation;
	unsigned char running;
} TTimer;

TTimer *TTimerNew(void);
static inline void TTimerInit(TTimer *t) {memset(t,0,sizeof(TTimer));}
static inline void TTimerFree(TTimer *t) {free(t);}

static inline void TTimerStart(TTimer *t) {t->start = TTimeGetTime(); t->running = 1;}
static inline void TTimerStop(TTimer *t) {t->stop = TTimeGetTime(); t->running = 0; t->accumulation += t->stop-t->start;}
static inline void TTimerReset(TTimer *t) {TTimerInit(t);}

static inline double TTimerElapsed(TTimer *t) {if(t->running) TTimerStop(t); return t->stop-t->start;}
static inline double TTimerGetAccumulatedTime(TTimer *t) {if(t->running) TTimerStop(t); return t->accumulation; }

#endif
