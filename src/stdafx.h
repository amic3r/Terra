
#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

#ifndef __included_stdafx_h
#define __included_stdafx_h

#undef __cplusplus

#define NULL 0

#ifndef bool
#define bool unsigned char
#define false 0
#define true !false
#endif

#if defined(_WIN32) || defined(_WIN64)
#define _WINDOWS 1
#define inline _inline
#define snprintf _snprintf
#endif

#ifdef _MSC_VER
#if _MSC_VER <= 1200
typedef __int64 intptr_t;

// A very useless (and VERY annoying) warning on VC6
#pragma warning ( disable: 4786 )
#endif

#if _MSC_VER >= 1400
#define __STDC__ 1  // Enforces ANSI C compliance.

// __STDC__ disables the following definitions in the C headers
#define strdup _strdup
#define stricmp _stricmp
#endif

#pragma warning ( disable: 4018 ) // signed/unsigned mismatch
#pragma warning ( disable: 4267 ) // conversion from 'size_t' to 'int', possible loss of data
#endif


#ifdef _MSC_VER
#ifdef _DEBUG
//#define CATCH_MEMORY_LEAKS               // Catches memory leaks and reports them on exit.
#endif

// #	define _CRT_ENFORCE_DEPRECATE */   // Throws an error (instead of warning) on __declspec(deprecated)
#define MINIMUM_DEPRECATE_WARNINGS       // Disables the most frequent deprecation warnings.
// #	define NO_DEPRECATE_WARNINGS  */   // Disable all deprecation
#endif

// This one is interesting. Research what it does before using!
// #define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#ifdef _MSC_VER
#ifdef _CRT_ENFORCE_DEPRECATE
//		Error on deprecation
#pragma warning (error:4996)
#endif
#ifdef MINIMUM_DEPRECATE_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE                        // Disable string function deprecations.
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#ifdef NO_DEPRECATE_WARNINGS
#pragma warning (disable:4996)
#endif
#endif // _MSC_VER

#ifdef CATCH_MEMORY_LEAKS
#define _CRTDBG_MAP_ALLOC
#endif

#include <assert.h>

#ifdef _DEBUG
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

#ifdef _WINDOWS
#define _WIN32_WINDOWS 0x0500      // for IsDebuggerPresent
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#include <glib.h>

#include "define.h"

#include "debugging/debug.h"

#endif // __included_stdafx_h
