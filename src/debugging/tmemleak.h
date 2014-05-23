
#ifndef __included_terra_memoryleak_h
#define __included_terra_memoryleak_h

#undef _malloca
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#if defined(_WINDOWS) && defined(_DEBUG)
void TMemLeakPrint(char *_filename);
#endif

#endif
