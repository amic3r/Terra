
#include "stdafx.h"

#include "tdebug.h"

#ifdef _WINDOWS
#include <Windows.h>
#else
#include <assert.h>
#endif

#include <time.h>

void TDebugOutData(void *_data, size_t _dataLen)
{
	size_t i = 0;
	for(; i < _dataLen; ++i) {
		if(i % 16 == 0)	TLogWrite("\n");
		TLogWrite("%02x ",((unsigned char *)_data)[i]);
	}
	TLogWrite("\n\n");
}

#ifdef _DEBUG
void TDebugAssert(unsigned char _condition)
{
	if(!_condition) {
#ifdef _WINDOWS
		ShowCursor(1);
		TLogReport(T_LOG_ERROR,"TDebugAssert","condition failed.");
#endif
		abort();
	}
}
#endif // _DEBUG

void TReleaseAssertFailed(const char *_msg, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, _msg);
	vsprintf(buf, _msg, ap);

#ifdef _WINDOWS
	ShowCursor(1);
	MessageBox(NULL, buf, "Fatal Error", MB_OK);
#else
	fputs(buf, stderr);
#endif

#ifndef _DEBUG
	TGenerateBlackBox(buf);
#endif // _DEBUG
	abort();
}

unsigned *getRetAddress(unsigned *mBP)
{
#ifdef _WINDOWS
	unsigned *retAddr;

	__asm {
		mov eax, [mBP]
		mov eax, ss:[eax+4];
		mov [retAddr], eax
	}

	return retAddr;
#else
	unsigned **p =(unsigned **)mBP;
	return p[1];
#endif
}

void TGenerateBlackBox(char *_msg)
{
	time_t timet = time(0);
	struct tm *thetime = localtime(&timet);
	unsigned *framePtr = 0;
	unsigned *previousFramePtr = 0;

	TLogWrite("=========================\n");
	TLogWrite("=   BLACK BOX REPORT    =\n");
	TLogWrite("=========================\n\n");

	TLogWrite("Date %d:%d, %d/%d/%d\n\n", thetime->tm_hour, thetime->tm_min, thetime->tm_mday, thetime->tm_mon+1, thetime->tm_year+1900);

	if(_msg)TLogWrite("ERROR : '%s'\n", _msg);
#ifndef _MACOSX				
	// For MacOSX, suggest Smart Crash Reports: http://smartcrashreports.com/
	// Print stack trace
	// Get our frame pointer, chain upwards
	TLogWrite("\n");
	TLogWrite("=========================\n");
	TLogWrite("=      STACKTRACE       =\n");
	TLogWrite("=========================\n\n");

#ifdef _WINDOWS
	__asm { mov [framePtr], ebp }
#else
	asm(
		"movl %%ebp, %0;"
		:"=r"(framePtr)
		);
#endif
	while(framePtr) {
		TLogWrite("retAddress = %p\n", getRetAddress(framePtr));
		framePtr = *(unsigned **)framePtr;

		// Frame pointer must be aligned on a
		// DWORD boundary. Bail if not so.
		if((unsigned long)framePtr & 3)	break;
		if(framePtr <= previousFramePtr) break;

		// Can two DWORDs be read from the supposed frame address?
#ifdef _WINDOWS
		if(IsBadWritePtr(framePtr, sizeof(PVOID)*2)|| IsBadReadPtr(framePtr, sizeof(PVOID)*2))
			break;
#endif

		previousFramePtr = framePtr;
	}
#endif // TARGET_OS_MACOSX
}
