
#include "stdafx.h"

#include "twriter.h"

#include "tthread.h"

#define LINE_ENDING_LENGTH 4
#define DEFAULT_COMMENT_FORMAT "#","\n"
#define NUM_DEFAULT_FORMAT 2

struct _CommentFormat {
	char begin[SIZE_COMMENT_STRING];
	char end[SIZE_COMMENT_STRING];
};

struct _TWriter {
	char lineending[LINE_ENDING_LENGTH];
	struct _CommentFormat commentFormat[NUM_COMMENT_FORMATS];

#ifndef __GNUC__
	TMutex *iomut;
#endif

	FILE *file;
};

TWriter *TWriterFileNew(const char *filename, unsigned char binary)
{
	return TWriterFilePointerNew(fopen(filename,binary ? "wb" : "w"));
}

TWriter *TWriterFilePointerNew(FILE *file)
{
	TWriter *w;
	const char *formats[] = {DEFAULT_COMMENT_FORMAT};
	
	if(!file) return 0;
	
	w = (TWriter *) malloc(sizeof(TWriter));
	if(!w) return 0;
	
	w->file = file;

#if defined (_MSC_VER) || defined (_WINDOWS)
	TWriterSetLineEnding(w,"\r\n");
#else
	TWriterSetLineEnding(w,"\n");
#endif
	
	TWriterSetCommentFormats(w,formats,NUM_DEFAULT_FORMAT);

#ifndef __GNUC__
	w->iomut = TMutexNew(T_MUTEX_RECURSIVE);
#endif

	return w;
}

void TWriterFree(TWriter *w,unsigned char close)
{
	if(w) {
		if(close) fclose(w->file);
#ifndef __GNUC__
		TMutexFree(w->iomut);
#endif

		free(w);
	}
}

void TWriterSetLineEnding(TWriter *w,const char *lineending)
{
	if(w && lineending) {
		strncpy(w->lineending,lineending,LINE_ENDING_LENGTH);
		w->lineending[LINE_ENDING_LENGTH-1] = '\0';
	}
}

void TWriterSetCommentFormats(TWriter *w,const char **_commentFormats,unsigned int size)
{
	if(!w || size & 1) return;

	{
		unsigned int i = 0,j = 0;
		for(;i < size && j < NUM_COMMENT_FORMATS; i+=2, ++j) {
			strncpy(w->commentFormat[j].begin,_commentFormats[i],SIZE_COMMENT_STRING-1);
			strncpy(w->commentFormat[j].end,_commentFormats[i+1],SIZE_COMMENT_STRING-1);
		}

		for(; j < NUM_COMMENT_FORMATS; ++j) {
			strncpy(w->commentFormat[j].begin,"\0",SIZE_COMMENT_STRING-1);
			strncpy(w->commentFormat[j].end,"\0",SIZE_COMMENT_STRING-1);
		}
	}
}

int TWriterWrite8(TWriter *w,unsigned char data)
{
	int result = 0;

	if(w) {
#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned char), w->file);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}

	return result;
}

int TWriterWrite16(TWriter *w,unsigned short data)
{
	int result = 0;

	if(w) {
#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned short), w->file);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}

	return result;
}

int TWriterWrite32(TWriter *w,unsigned int data)
{
	int result = 0;

	if(w) {
#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned int), w->file);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}

	return result;
}

int TWriterWrite64(TWriter *w,unsigned long long data)
{
	int result = 0;

	if(w) {
#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned long long), w->file);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}

	return result;
}

int TWriterWriteBlock(TWriter *w,unsigned int _count, const void *_buffer)
{
	int result = 0;

	if(w) {
#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		result = fwrite(_buffer, 1, _count, w->file);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}

	return result;
}

void TWriterWrite(TWriter *w,const char *format,...)
{
	if(w && format) {
		va_list args;
		va_start(args, format);

#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		vfprintf(w->file,format,args);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
		va_end(args);
	}
}

void TWriterWriteLine(TWriter *w,const char *format,...)
{
	if(w && format) {
		
		va_list args;
		va_start(args, format);

#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		vfprintf(w->file,format,args);
		fprintf(w->file, "%s", w->lineending);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
		va_end(args);
	}
}

void TWriterWriteComment(TWriter *w,unsigned int commentindex,const char *format,...)
{
	if(w && format) {
		struct _CommentFormat *comment;
		va_list args;

		if(commentindex > NUM_COMMENT_FORMATS) return;
		comment = &w->commentFormat[commentindex];
		if (comment->begin[0] == '\0') return;
		
		va_start(args, format);

#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		fprintf(w->file, "%s", comment->begin);
		vfprintf(w->file,format,args);
		fprintf(w->file, "%s",comment->end);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
		va_end(args);
	}
}

void TWriterWriteV(TWriter *w,const char *format, const va_list *list)
{
	if(w && format) {

#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		vfprintf(w->file,format,*list);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}
}

void TWriterWriteLineV(TWriter *w,const char *format, const va_list *list)
{
	if(w && format) {

#ifndef __GNUC__
		TMutexLock(w->iomut);
#endif
		vfprintf(w->file,format,*list);
		fprintf(w->file, "%s", w->lineending);

#ifndef __GNUC__
		TMutexUnlock(w->iomut);
#endif
	}
}
