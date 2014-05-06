
#ifndef __included_terra_time_h
#define __included_terra_time_h

void ttime_initialise();
double ttime_get_time();

void ttime_compute_time();
double ttime_fetch_time();

//------------- TTimer ---------------//

typedef struct TTimer {
	double start, stop;
	double accumulation;
	bool running;
} TTimer;

TTimer *ttimer_new(void);
static inline void ttimer_init(TTimer *t) {memset(t,0,sizeof(TTimer));}
static inline void ttimer_free(TTimer *t) {free(t);}

static inline void ttimer_start(TTimer *t) {t->start = ttime_get_time(); t->running = true;}
static inline void ttimer_stop(TTimer *t) {t->stop = ttime_get_time(); t->running = false; t->accumulation += t->stop-t->start;}
static inline void ttimer_reset(TTimer *t) {ttimer_init(t);}

static inline double ttimer_elapsed(TTimer *t) {if(t->running) ttimer_stop(t); return t->stop-t->start;}
static inline double ttimer_get_accumulated_time(TTimer *t) {if(t->running) ttimer_stop(t); return t->accumulation; }

#endif
