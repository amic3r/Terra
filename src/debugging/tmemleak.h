
#ifndef __included_terra_memoryleak_h
#define __included_terra_memoryleak_h

#ifdef _DEBUG
#undef _malloca
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

void TMemLeakPrint(char *_filename);
#endif

#endif
