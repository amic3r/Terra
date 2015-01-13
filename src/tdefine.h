
#ifndef __included_terra_define_h
#define __included_terra_define_h

#undef  NULL
#define NULL 0

#if defined(_WIN32) || defined(_WIN64)
#define _WINDOWS 1
#define inline _inline
#define snprintf _snprintf
#define _USE_MATH_DEFINES 1
#elif defined(__linux) || defined(__linux__)
#define _LINUX 1
#endif

#ifdef _MSC_VER
#define __STDC__ 1  // Enforces ANSI C compliance.

// __STDC__ disables the following definitions in the C headers
#define strdup _strdup
#define stricmp _stricmp
#endif

#define UNREFERENCED_PARAMETER(P) (P)

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

typedef void (*TFreeFunc) (void *);

typedef void *(*TCopyFunc) (void *);

typedef void (*TIterFunc) (void *);
typedef void *(*TDataIterFunc) (void *,void *);

typedef void (*TPairIterFunc) (void *,void *);
typedef void *(*TDataPairIterFunc) (void *, void *, void *);

typedef int (*TCompareFunc) (const void *,const void *);

#endif
