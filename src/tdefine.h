
#ifndef __included_terra_define_h
#define __included_terra_define_h

/**
* Terra Definition
*
*   The purpose of this file is to setup basic definitions
*   used throughout the library.
*
*/


#if defined(_WIN32) || defined(_WIN64)
#define _WINDOWS 1
#define _USE_MATH_DEFINES 1

#define inline _inline
#define snprintf _snprintf
#elif defined(__linux) || defined(__linux__)
#define _LINUX 1
#endif

#ifdef _MSC_VER
#define __STDC__ 1  // Enforces ANSI C compliance.

// __STDC__ disables the following definitions in the C headers
#define strdup _strdup
#define stricmp _stricmp
#endif

#ifndef __unused_param
#define __unused_param __attribute__ ((unused))
#endif

#define TMAX(a,b)     ((a > b) ?  a : b)
#define TMIN(a,b)     ((a < b) ?  a : b)
#define TABS(a)	      ((a < 0) ? -a : a)

#define TCLAMP(x,l,h) ((x > h) ? h : ((x < l) ? l : x))

#define TSWAP(x,y)	{ \
					unsigned char st[sizeof(x) == sizeof(y) ? sizeof(x) : -1]; \
					memcpy(st,&y,sizeof(x)); \
					memcpy(&y,&x,sizeof(x)); \
					memcpy(&x,st,sizeof(x)); \
					}

#define TSWAPT(x,y,T)	{ \
					T p = x; \
					x = y; \
					y = p; \
					}

typedef void * TPtr;
typedef const void * TCPtr;

typedef signed char TInt8;
typedef unsigned char TUInt8;
typedef signed short TInt16;
typedef unsigned short TUInt16;
typedef signed int TInt32;
typedef unsigned int TUInt32;
typedef signed long long TInt64;
typedef unsigned long long TUInt64;

typedef void(*TFreeFunc) (TPtr);

typedef TPtr (*TCopyFunc) (TPtr);

typedef void (*TIterFunc) (TPtr);
typedef TPtr (*TDataIterFunc) (TPtr, TPtr);

typedef void (*TPairIterFunc) (TPtr, TPtr);
typedef TPtr (*TDataPairIterFunc) (TPtr, TPtr, TPtr);

typedef int (*TCompareFunc) (TCPtr, TCPtr);

typedef int (*TThreadFunc) (TPtr);

enum T_DATA_TYPE {
	T_DATA_UNKNOWN = 0,
	T_DATA_CONST_UNKNOWN,

	T_DATA_NULL,

	T_DATA_INT8,
	T_DATA_UINT8,
	T_DATA_INT16,
	T_DATA_UINT16,
	T_DATA_INT32,
	T_DATA_UINT32,
	T_DATA_INT64,
	T_DATA_UINT64,

	T_DATA_CONST_STRING,
	T_DATA_STRING,
	T_DATA_CHAR,

	T_DATA_FLOAT,
	T_DATA_DOUBLE,

	T_DATA_AMOUNT,
};

#endif
