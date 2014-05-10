
#ifndef __included_terra_filesystem_h
#define __included_terra_filesystem_h

#include "tdefine.h"
#include "structure/tlist.h"
#include "io/treader.h"

void TIOInitialize(void);
void TIODestroy(void);

void TIOParseArchive(const char *filename);
TSList *TIOListArchive(const char *_dir, const char *_filter, unsigned char fullFilename);

FILE *TIOGetFile(const char *filename,const char *mode);
TReader *TIOGetReader(const char *filename,unsigned char binarymode);
unsigned char *TIOGetBufferedFile(const char *filename,unsigned char binarymode,unsigned int *size);

void TIOAddSearchPath(const char *path);
void TIOClearSearchPath(void);

static inline const char *TIOGetApplicationPath(void) { return "."; }

#endif
