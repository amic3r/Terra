
#include "stdafx.h"

#include "tio.h"

#include "thread/tthread.h"

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

	if(!mode) mode = "r";

	found = (char *) TSListForeachData(searchpaths,testpath,(void *) filename);
	if(found) {
		FILE *f = fopen(found,mode);
		free(found);
		return f;
	}

	//TODO : check in archives

	return 0;
}

TReader *TIOGetReader(const char *filename,unsigned char binarymode)
{
	TReader *reader = 0;

	FILE *f = TIOGetFile(filename,binarymode ? "rb" : "r");
	if(f) reader = TReaderFilePointerNew(f);

	return reader;
}

unsigned char *TIOGetBufferedFile(const char *filename,unsigned char binarymode,unsigned int *size)
{
	unsigned char *buffer = 0;
	unsigned int finalsize = 0;
	
	TReader *r = TIOGetReader(filename,binarymode);
	if(!r) return 0;

	finalsize = TReaderSize(r);
	buffer = (unsigned char *) malloc(sizeof(unsigned char) * finalsize);
	*size = TReaderReadBlock(r,finalsize, buffer);

	TReaderFree(r,1);

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
