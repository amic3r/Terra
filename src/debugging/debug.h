
#ifndef __included_terra_debug_utils_h
#define __included_terra_debug_utils_h

void AppDebugOutPath(const char *_path);			// Sets the output path

void AppDebugOut(char *_msg, ...);					// Outputs to Devstudio output pane
void AppDebugOutData(void *_data, int _dataLen);	// Dumps the data as a hex table

#ifdef _DEBUG
void AppDebugAssert(unsigned char _condition);		// Does nothing in Release builds
#else
#define AppDebugAssert(x) {}
#endif // _DEBUG

void AppReleaseAssertFailed(const char *_fmt, ...);	// Same as DebugAssert in Debug builds
void AppGenerateBlackBox(char *_filename, char *_msg);

#ifdef _MSC_VER
#define AppReleaseAssert(cond, ...) \
	do { \
		if(!(cond)) \
			AppReleaseAssertFailed(__VA_ARGS__); \
	} while(0)
#else // GCC
#define AppReleaseAssert(cond, fmt, args...) \
	do { \
		if(!(cond)) \
			AppReleaseAssertFailed(fmt, ## args); \
	} while(0)
#endif

#define AppAssert(x)		AppReleaseAssert((x),			\
							"Assertion failed : '%s'\n\n"	\
							"%s\nline number %d",			\
							#x, __FILE__, __LINE__)

#define AppAbort(x)			AppReleaseAssert(0,				\
							"Abort : '%s'\n\n"				\
							"%s\nline number %d",			\
							x, __FILE__, __LINE__)

void AppDebugOutRedirect(const char *_filename);

#endif
