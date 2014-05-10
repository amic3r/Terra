
#ifndef _included_terra_particle_h
#define _included_terra_particle_h

//------------- Thread --------------//

typedef struct _TThread TThread;

TThread *TThreadCreate(size_t (*fn)(void *), void *data);
size_t TThreadJoin(TThread *t);

//------------- Mutex ---------------//

#define PARTICLE_MUTEX_TYPE_NORMAL     0
#define PARTICLE_MUTEX_TYPE_RECURSIVE  1
#define PARTICLE_MUTEX_TYPE_ERRORCHECK 2
#define PARTICLE_MUTEX_TYPE_READWRITE  3

typedef struct _TMutex TMutex;

TMutex *TMutexNew(unsigned int type);
void TMutexFree(TMutex *m);

void TMutexLock(TMutex *m);
void TMutexUnlock(TMutex *m);

//------- Condition Variable --------//

typedef struct _TCV TCV;

TCV *TCVNew(TMutex *m);
void TCVFree(TCV *v);

void TCVSleep(TCV *v);
void TCVWake(TCV *v);
void TCVWakeSingle(TCV *v);

#endif
