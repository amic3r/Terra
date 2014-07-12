
#ifndef __included_stdafx_h
#define __included_stdafx_h

#undef __cplusplus

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE                        // Disable string function deprecations.
#define _CRT_NONSTDC_NO_DEPRECATE
#endif // _MSC_VER

#include "tdefine.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdarg.h>

#ifndef _WINDOWS
#include <unistd.h>
#include <sys/types.h>
#endif

#endif // __included_stdafx_h
