
#include "stdafx.h"

#include "tio.h"

#include "tthread.h"

#include "structure/tlist.h"

#ifdef _WINDOWS
#include <shlobj.h>
#endif

#include "utility/tfilesys.h"

#include "debugging/tdebug.h"

static TSList *searchpaths = 0;

void TIOInitialize(void)
{
	searchpaths = 0;
	TIOAddSearchPath(TIOGetApplicationPath());
}

void TIODestroy(void)
{
	TSListFree(searchpaths,free);
	searchpaths = 0;
}

void TIOParseArchive(const char *filename)
{
	TAbort("File System: parse archive function not implemented");
}

TSList *TIOListArchive(const char *_dir, const char *_filter, unsigned char fullFilename)
{
	TAbort("File System: list archive function not implemented");
	return 0;
}

void *testpath(void *searchpath, void *filename)
{
	char *fullFilename = TFileSysConcatPaths((const char *) searchpath, filename,NULL);
	if(TFileSysFileExists(fullFilename)) return fullFilename;

	free(fullFilename);

	return 0;
}

FILE *TIOGetFile(const char *filename,const char *mode)
{
	char *found = 0;
	if(!filename) return 0;

	if(!mode) mode = "rb";

	found = (char *) TSListForeachData(searchpaths,testpath,(void *) filename);
	if(found) {
		FILE *f = fopen(found,mode);
		free(found);
		return f;
	}

	//TODO : check in archives

	return 0;
}

TRW *TIOGetRW(const char *filename,const char *mode)
{
	TRW *trw = 0;

	FILE *f = TIOGetFile(filename,mode);
	if(f) trw = TRWFromFilePointer(f,1);

	return trw;
}

char *TIOGetBufferedFile(const char *filename, const char *mode, unsigned int *size)
{
	char *buffer = 0;
	unsigned int finalsize = 0;
	
	TRW *trw = TIOGetRW(filename,mode);
	if(!trw) return 0;

	finalsize = TRWSize(trw);
	buffer = (char *) malloc(sizeof(char) * finalsize);
	*size = TRWReadBlock(trw, buffer, finalsize);

	TRWFree(trw);

	return buffer;
}

void TIOAddSearchPath(const char *path)
{
	if(!searchpaths) searchpaths = TSListNew();

	TSListAppend(searchpaths,strdup(path));
}

void TIOClearSearchPath(void)
{
	TSListFree(searchpaths,free);
	TIOAddSearchPath(TIOGetApplicationPath());
}
