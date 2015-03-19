
#ifndef _included_terra_particle_h
#define _included_terra_particle_h

//------------- Thread --------------//

typedef struct _TThread TThread;

TThread *TThreadCreate(int (*fn)(void *), void *data);
int TThreadJoin(TThread *t);

void TThreadSleep(int ms);

//------------- Mutex ---------------//

enum T_MUTEX_TYPET {
	T_MUTEX_NORMAL = 0,
	T_MUTEX_RECURSIVE,
	T_MUTEX_ERRORCHECK,
	T_MUTEX_READWRITE,
};

typedef struct _TMutex TMutex;

TMutex *TMutexNew(int type);
void TMutexFree(TMutex *m);

void TMutexLock(TMutex *m);
void TMutexUnlock(TMutex *m);

//------- Condition Variable --------//

typedef struct _TCV TCV;

TCV *TCVNew(TMutex *m);
void TCVFree(TCV *v);

int  TCVSleep(TCV *v, size_t msec);
void TCVWake(TCV *v);
void TCVWakeSingle(TCV *v);

#endif
