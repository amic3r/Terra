
#ifndef __included_terra_memoryleak_h
#define __included_terra_memoryleak_h

#if defined(_MSC_VER) && defined(_DEBUG)
void AppPrintMemoryLeaks(char *_filename);
#endif

#endif
