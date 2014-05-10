
#include "stdafx.h"

#include "tthread.h"

#ifndef _WINDOWS
#include <pthread.h>
#else
#include <Windows.h>
#endif

//------------- Thread --------------//

struct _TThread {
#ifdef _WINDOWS
	HANDLE thread;
#else
	pthread_t thread;
#endif
};

typedef struct _ThreadFunction {
	size_t (*fn)(void *);
	void *data;
} ThreadFunction;

#ifdef _WINDOWS
DWORD WINAPI run_wrapper(LPVOID param) {
#else
void *run_wrapper(void *param) {
#endif
	ThreadFunction *tf = (ThreadFunction *) param;
	size_t (*fn)(void *) = tf->fn;
	void *data = tf->data;

	free(tf);
	fn(data);

	return 0;
}

TThread *TThreadCreate(size_t (*fn)(void *), void *data)
{
	ThreadFunction *tf = (ThreadFunction *) malloc(sizeof(ThreadFunction));
	TThread *t = (TThread *) malloc(sizeof(TThread));

	tf->fn = fn;
	tf->data = data;

#ifdef _WINDOWS
	t->thread = CreateThread(0,0,run_wrapper,tf,0,0);
#else
	pthread_create(&pt->thread, NULL,&run_wrapper,tf);
#endif

	return t;
}

size_t TThreadJoin(TThread *t)
{
	size_t retval = 0;
#ifdef _WINDOWS
	unsigned long wrv;
	WaitForSingleObject(t->thread, INFINITE);
	GetExitCodeThread(t->thread,&wrv);
	CloseHandle(t->thread);
	retval = wrv;
#else
	pthread_join(t->thread, (void **) &retval);
#endif
	free(t);

	return retval;
}

//------------- Mutex ---------------//

struct _TMutex {
#ifdef _WINDOWS
	CRITICAL_SECTION mutex;
#else
	pthread_mutexattr_t mutexAttr;
	pthread_mutex_t mutex;
#endif
	unsigned int type;
};

TMutex *TMutexNew(unsigned int type)
{
	TMutex *m = (TMutex *) malloc(sizeof(TMutex));
	m->type = type;

#ifdef _WINDOWS
	InitializeCriticalSection(&m->mutex);
#else
	int error = pthread_mutexattr_init(&m->mutexAttr);
	assert(error == 0);

	if(type == PARTICLE_MUTEX_TYPE_NORMAL) {
		type = PTHREAD_MUTEX_NORMAL;
	} else if (type == PARTICLE_MUTEX_TYPE_RECURSIVE) {
		type = PTHREAD_MUTEX_RECURSIVE;
	} else if (type == PARTICLE_MUTEX_TYPE_ERRORCHECK) {
		type = PTHREAD_MUTEX_ERRORCHECK;
	} else if (type == PARTICLE_MUTEX_TYPE_READWRITE) {
		type = PTHREAD_PROCESS_PRIVATE;
	}

	error = pthread_mutexattr_settype(&m->mutexAttr, type);
	assert(error == 0);
	error = pthread_mutex_init(&m->mutex, &m->mutexAttr);
	assert(error == 0);
#endif

	return m;
}

void TMutexFree(TMutex *m)
{
#ifdef _WINDOWS
	DeleteCriticalSection(&m->mutex);
#else
	pthread_mutex_destroy(&m->mutex);
	pthread_mutexattr_destroy(&m->mutexAttr);
#endif

	free(m);
}

void TMutexLock(TMutex *m)
{
#ifdef _WINDOWS
	EnterCriticalSection(&m->mutex);
#else
	assert(pthread_mutex_lock(&m->mutex) == 0);
#endif
}

void TMutexUnlock(TMutex *m)
{
#ifdef _WINDOWS
	LeaveCriticalSection(&m->mutex);
#else
	assert(pthread_mutex_unlock(&m->mutex) == 0);
#endif
}

//------- Condition Variable --------//

struct _TCV {
	TMutex *m;

#ifdef _WINDOWS
	CONDITION_VARIABLE var;
#else
	pthread_cond_t var;
#endif
};

TCV *TCVNew(TMutex *m)
{
	TCV *v = (TCV *) malloc(sizeof(TCV));
	v->m = m;

#ifdef _WINDOWS
	InitializeConditionVariable(&v->var);
#else
	pthread_cond_init(&v->var,0);
#endif

	return v;
}

void TCVFree(TCV *v)
{
	if(v) {
#ifndef _WINDOWS
		pthread_cond_destroy(&v->var);
#endif

		free(v);
	}
}

void TCVSleep(TCV *v)
{
#ifdef _WINDOWS
	SleepConditionVariableCS(&v->var, &v->m->mutex, INFINITE);
#else
	pthread_cond_wait(&v->var,&v->m->mutex);
#endif
}

void TCVWake(TCV *v)
{
#ifdef _WINDOWS
	WakeAllConditionVariable(&v->var);
#else
	pthread_cond_broadcast(&v->var);
#endif
}

void TCVWakeSingle(TCV *v)
{
#ifdef _WINDOWS
	WakeConditionVariable(&v->var);
#else
	pthread_cond_signal(&v->var);
#endif
}
