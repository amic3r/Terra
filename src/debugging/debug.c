
#include "stdafx.h"

static char *s_debugOutRedirect = NULL;

static char *debugPath = NULL;

void AppDebugOutPath(const char *_path)
{
	size_t pathlen = strlen(_path);

	free(debugPath);

	if(_path[pathlen-1] != '/') {
		debugPath = (char *) malloc(sizeof(char) * (pathlen + 2));
		snprintf(debugPath,pathlen + 2,"%s/",_path);
	} else {
		debugPath = strdup(_path);
	}
}

char *AppDebugLogPath(const char *_filename)
{
	if(debugPath) {
		size_t len = strlen(debugPath) + strlen(_filename);
		char *fullpath = (char *) malloc(sizeof(char) * len + 1);
		snprintf(fullpath, len + 1,"%s%s",debugPath,_filename);
		return fullpath;
	}

	return 0;
}

void AppDebugOutRedirect(const char *_filename)
{
	// re-using same log file is a no-op
	char *res = AppDebugLogPath(_filename);

	if(res && (!s_debugOutRedirect || strcmp(s_debugOutRedirect, res))) {
		FILE *file = 0;
		free(s_debugOutRedirect);
		s_debugOutRedirect = res;

		// Check we have write access, and clear the file
		file = fopen(s_debugOutRedirect, "w");

		if(!file) {
			free(s_debugOutRedirect);
			s_debugOutRedirect = NULL;

			AppDebugOut("Failed to open %s for writing\n", _filename);
		} else
			fclose(file);
	} else
		free(res);
}

void AppDebugOut(char *_msg, ...)
{
	va_list ap;
	va_start(ap, _msg);

	if(s_debugOutRedirect) {
		FILE *file = fopen(s_debugOutRedirect, "a");
		if(!file) {
			printf("Failed to open %s for writing\n", s_debugOutRedirect);
			free(s_debugOutRedirect);
			s_debugOutRedirect = NULL;
			vprintf(_msg,ap);
		} else {
			vfprintf(file, _msg,ap);
			fclose(file);
		}
	} else
		vprintf(_msg,ap);
}

void AppDebugOutData(void *_data, int _dataLen)
{
	int i = 0;
	for(; i < _dataLen; ++i) {
		if(i % 16 == 0)				AppDebugOut("\n");
		AppDebugOut("%02x ",((unsigned char *)_data)[i]);
	}
	AppDebugOut("\n\n");
}

#ifdef _DEBUG
void AppDebugAssert(unsigned char _condition)
{
	if(!_condition) {
#ifdef _WINDOWS
		ShowCursor(1);
		_ASSERT(_condition);
#else
		abort();
#endif
	}
}
#endif // _DEBUG

void AppReleaseAssertFailed(const char *_msg, ...)
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
	AppGenerateBlackBox("blackbox.txt",buf);
	throw;
	//exit(-1);
#else
#ifdef _WINDOWS
	_ASSERT(0);
#else
	abort();
#endif
#endif // _DEBUG
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

void AppGenerateBlackBox(char *_filename, char *_msg)
{
	char *path = AppDebugLogPath(_filename);
	FILE *_file = fopen(path, "wt");
	if(_file) {
		time_t timet;
		struct tm *thetime = 0;
		unsigned *framePtr = 0;
		unsigned *previousFramePtr = 0;

		fprintf(_file, "=========================\n");
		fprintf(_file, "=   BLACK BOX REPORT    =\n");
		fprintf(_file, "=========================\n\n");

		timet = time(0);
		thetime = localtime(&timet);

		fprintf(_file, "Date %d:%d, %d/%d/%d\n\n", thetime->tm_hour, thetime->tm_min, thetime->tm_mday, thetime->tm_mon+1, thetime->tm_year+1900);

		if(_msg)fprintf(_file, "ERROR : '%s'\n", _msg);
#ifndef TARGET_OS_MACOSX				
		// For MacOSX, suggest Smart Crash Reports: http://smartcrashreports.com/
		// Print stack trace
		// Get our frame pointer, chain upwards
		fprintf(_file, "\n");
		fprintf(_file, "=========================\n");
		fprintf(_file, "=      STACKTRACE       =\n");
		fprintf(_file, "=========================\n\n");

#ifdef _WINDOWS
		__asm { mov [framePtr], ebp }
#else
		asm(
			"movl %%ebp, %0;"
			:"=r"(framePtr)
			);
#endif
		while(framePtr) {
		
			fprintf(_file, "retAddress = %p\n", getRetAddress(framePtr));
			framePtr = *(unsigned **)framePtr;

			// Frame pointer must be aligned on a
			// DWORD boundary. Bail if not so.
			if((unsigned long)framePtr & 3)				break;
			if(framePtr <= previousFramePtr)				break;

			// Can two DWORDs be read from the supposed frame address?
#ifdef _WINDOWS
			if(IsBadWritePtr(framePtr, sizeof(PVOID)*2)|| IsBadReadPtr(framePtr, sizeof(PVOID)*2))
				break;
#endif

			previousFramePtr = framePtr;
		}
#endif // TARGET_OS_MACOSX
		fclose(_file);
	}
	free(path);
}
