
#ifndef __included_terra_define_h
#define __included_terra_define_h

#undef  NULL
#define NULL 0

#if defined(_WIN32) || defined(_WIN64)
#define _WINDOWS 1
#define inline _inline
#define snprintf _snprintf
#endif

#if _MSC_VER >= 1400
#define __STDC__ 1  // Enforces ANSI C compliance.

// __STDC__ disables the following definitions in the C headers
#define strdup _strdup
#define stricmp _stricmp
#endif

#undef  max
#define max(a,b)     ((a > b) ? a : b)

#undef  min
#define min(a,b)     ((a < b) ? a : b)

#undef	abs
#define abs(a)	     ((a < 0) ? -a : a)

#undef	clamp
#define clamp(x,l,h) ((x > h) ? h : ((x < l) ? l : x))

#undef swap
#define swap(x,y)	do { \
					unsigned char st[sizeof(x) == sizeof(y) ? sizeof(x) : -1]; \
					memcpy(st,&y,sizeof(x)); \
					memcpy(&y,&x,sizeof(x)); \
					memcpy(&x,st,sizeof(x)); \
					} while(0)


typedef void (*TFreeFunc) (void *);

typedef void *(*TCopyFunc) (void *);

typedef void (*TIterFunc) (void *);
typedef void *(*TDataIterFunc) (void *,void *);

typedef int (*TCompareFunc) (void *,void *);

#endif
