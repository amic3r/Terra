
#ifndef __included_terra_debug_h
#define __included_terra_debug_h

#include "tlog.h"

void TDumpData(void *_data, size_t _dataLen);	// Dumps data as a hex table

#ifdef _DEBUG
void TDebugAssert(unsigned char _condition);		// Does nothing in Release builds
#else
#define TDebugAssert(x) {}
#endif // _DEBUG

void TReleaseAssertFailed(const char *_fmt, ...);	// Same as DebugAssert in Debug builds
void TGenerateBlackBox(char *_msg);

#ifdef _MSC_VER
#define TReleaseAssert(cond, ...) \
	do { \
		if(!(cond)) \
			TReleaseAssertFailed(__VA_ARGS__); \
	} while(0)
#else // GCC
#define TReleaseAssert(cond, fmt, args...) \
	do { \
		if(!(cond)) \
			TReleaseAssertFailed(fmt, ## args); \
	} while(0)
#endif

#define TAssert(x)		TReleaseAssert((x),				\
						"Assertion failed : '%s'\n\n"	\
						"%s\nline number %d",			\
						#x, __FILE__, __LINE__)

#define TAbort(x)		TReleaseAssert(0,				\
						"Abort : '%s'\n\n"				\
						"%s\nline number %d",			\
						x, __FILE__, __LINE__)

#endif
