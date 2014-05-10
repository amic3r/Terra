
#include "stdafx.h"

#include "treader.h"

#include "thread/tthread.h"

#include "debugging/tdebug.h"

#define DEFAULT_SEPERATOR_CHARS " \t\n\r:,"

#define DEFAULT_COMMENT_FORMAT "#","\n"
#define NUM_DEFAULT_FORMAT 2

#define INIT_LINE_SIZE 128
#define LINE_SIZE_LIMIT 2048

struct _CommentFormat {
	char begin[SIZE_COMMENT_STRING];
	char end[SIZE_COMMENT_STRING];
};

struct _TReader {
	char *line;
	unsigned int linelen;			// string length without '\0'

	char separatorChars[SEPARATOR_CHAR_LENGTH];
	struct _CommentFormat commentFormat[NUM_COMMENT_FORMATS];

	unsigned int lineNum;

	unsigned int offset;
	unsigned int tokenend;

#ifndef __GNUC__
	TMutex *iomut;
#endif

	FILE *file;
};

unsigned char TReaderGrow(TReader *r,unsigned int size)
{
	char *biggerline = (char*) realloc(r->line, r->linelen+1+size);
	if(!biggerline) return 1;

	r->line = biggerline;
	r->linelen += size;

	return 0;
}

unsigned int TReaderDoubleSize(TReader *r)
{
	if(!r) return 1;

	if(r->linelen == 0) {
		r->line = (char*) malloc(INIT_LINE_SIZE);
		if(!r->line) return 1;

		r->linelen = INIT_LINE_SIZE-1;
		return 0;
	}

	if(r->linelen + 1 >= LINE_SIZE_LIMIT) return 2;
	
	return TReaderGrow(r,r->linelen+1);
}

TReader *TReaderFileNew(const char *filename, unsigned char binary)
{
	return TReaderFilePointerNew(fopen(filename,binary ? "rb" : "r"));
}

TReader *TReaderFilePointerNew(FILE *file)
{
	TReader *r;
	const char *formats[] = {DEFAULT_COMMENT_FORMAT};
	
	if(!file) return 0;
	
	r = (TReader *) malloc(sizeof(TReader));
	if(!r) return 0;
	
	r->file = file;

	r->offset = r->linelen = r->lineNum = r->tokenend = 0;
	r->line = 0;

	strncpy(r->separatorChars, DEFAULT_SEPERATOR_CHARS, SEPARATOR_CHAR_LENGTH-1);
	TReaderSetCommentFormats(r,formats,NUM_DEFAULT_FORMAT);

#ifndef __GNUC__
	r->iomut = TMutexNew(PARTICLE_MUTEX_TYPE_RECURSIVE);
#endif

	return r;
}

void TReaderFree(TReader *r, unsigned char close)
{
	if(r) {
		if(close) fclose(r->file);

		free(r->line);
		r->line = 0;

#ifndef __GNUC__
		TMutexFree(r->iomut);
#endif

		free(r);
	}
}

unsigned int TReaderSize(TReader *r)
{
	unsigned int now = 0,size = 0;
	if(!r) return 0;

	now = TReaderTell(r);
	fseek(r->file, 0, SEEK_END);
	size = TReaderTell(r);
	fseek(r->file, now, SEEK_SET);

	return size;
}

int TReaderSeek(TReader *r, int _offset, int _origin)
{
	if(!r) return 1;

	if(_origin == SEEK_CUR)
		r->offset += _offset;
	else if(_origin == SEEK_END)
		r->offset = TReaderSize(r) - _offset;
	else if(_origin == SEEK_SET)
		r->offset = _offset;

	return fseek(r->file, _offset, _origin);
}

unsigned int TReaderTell(TReader *r)
{
	if(!r) return 0;

	return ftell(r->file);
}

unsigned char TReaderEof(TReader *r)
{
	if(!r) return 1;

	return feof(r->file);
}

void TReaderSetSeparators(TReader *r,const char *_seperatorChars)
{
	if(!r) return;

	strncpy(r->separatorChars, _seperatorChars, SEPARATOR_CHAR_LENGTH-1);
	r->separatorChars[SEPARATOR_CHAR_LENGTH-1] = '\0';
}

void TReaderSetCommentFormats(TReader *r,const char **_commentFormats,unsigned int size)
{
	if(!r || size & 1) return;

	{
		unsigned int i = 0,j = 0;
		for(;i < size && j < NUM_COMMENT_FORMATS; i+=2, ++j) {
			strncpy(r->commentFormat[j].begin,_commentFormats[i],SIZE_COMMENT_STRING-1);
			strncpy(r->commentFormat[j].end,_commentFormats[i+1],SIZE_COMMENT_STRING-1);
		}

		for(; j < NUM_COMMENT_FORMATS; ++j) {
			strncpy(r->commentFormat[j].begin,"\0",SIZE_COMMENT_STRING-1);
			strncpy(r->commentFormat[j].end,"\0",SIZE_COMMENT_STRING-1);
		}
	}
}

unsigned int TReaderGetLineNbr(TReader *r)
{
	return r ? r->lineNum : 0;
}

unsigned char TReaderRead8(TReader *r)
{
	unsigned char content = EOF;

	if(r) {
#ifndef __GNUC__
		TMutexLock(r->iomut);
#endif

		fread(&content, 1, sizeof(unsigned char), r->file);

#ifndef __GNUC__
		TMutexUnlock(r->iomut);
#endif
		r->offset += 1;
	}

	return content;
}

unsigned short TReaderRead16(TReader *r)
{
	unsigned short content = EOF;

	if(r) {
#ifndef __GNUC__
		TMutexLock(r->iomut);
#endif

		fread(&content, 1, sizeof(unsigned short), r->file);

#ifndef __GNUC__
		TMutexUnlock(r->iomut);
#endif

		r->offset += 2;
	}

	return content;
}

unsigned int TReaderRead32(TReader *r)
{
	unsigned int content = EOF;

	if(r) {
#ifndef __GNUC__
		TMutexLock(r->iomut);
#endif

		fread(&content, 1, sizeof(unsigned int), r->file);

#ifndef __GNUC__
		TMutexUnlock(r->iomut);
#endif
		r->offset += 4;
	}

	return content;
}

unsigned long long TReaderRead64(TReader *r)
{
	unsigned long long content = EOF;

	if(r) {
#ifndef __GNUC__
		TMutexLock(r->iomut);
#endif

		fread(&content, 1, sizeof(unsigned long long), r->file);

#ifndef __GNUC__
		TMutexUnlock(r->iomut);
#endif
		r->offset += 8;
	}

	return content;
}

unsigned int TReaderReadBlock(TReader *r,unsigned int _count, unsigned char *_buffer)
{
	unsigned int result = 0;
	if(!r || !_buffer || _count == 0) return 0;

#ifndef __GNUC__
		TMutexLock(r->iomut);
#endif

	result = fread(_buffer, 1, _count, r->file);

#ifndef __GNUC__
		TMutexUnlock(r->iomut);
#endif

	r->offset += result;
	_buffer[result++] = '\0';

	return result;
}

static inline unsigned char TReaderNextLine(TReader *r)
{
	unsigned char found = 0;
	unsigned int result = 0;
	char *ptr = r->line;

	if(feof(r->file)) {
		*ptr = 0;
		return 0;
	}

	result = fread(ptr, 1, r->linelen, r->file);
	ptr[result] = 0;

	while(!feof(r->file)) {
		unsigned int oldLineLen = r->linelen;
		unsigned int status = TReaderDoubleSize(r);
		if(status == 0) {
			ptr += oldLineLen;
			result = fread(ptr,oldLineLen, 1, r->file);
			ptr[result] = 0;
		} else if(status == 2) {
			//limit reached - we cut the line here
			break;
		} else {
			//mem allocation error
			TAbort("Text Stream Reader: memory allocation failed");
		}
	}

	return 1;
}

static inline unsigned char TReaderFillBuffer(TReader *r)
{
	unsigned int res = 0;
	if(!r) return 0;

	r->offset = 0;

#ifndef __GNUC__
	TMutexLock(r->iomut);
#endif

	if(TReaderEof(r)) {
		*r->line = 0;
		return 0;
	}

	if(!r->line) TReaderDoubleSize(r);

	res = TReaderNextLine(r);

#ifndef __GNUC__
	TMutexUnlock(r->iomut);
#endif

	return res;
}

static inline unsigned char TReaderSkipSeparators(TReader *r)
{
	while(strchr(r->separatorChars,r->line[r->offset])) {
		if(r->line[r->offset] == '\n') r->lineNum++;
		if(++r->offset == r->linelen)
			if(!TReaderFillBuffer(r))
				return 0;
	}

	return 1;
}

static inline unsigned int TReaderFindTokenEnd(TReader *r)
{
	unsigned int tokenEnd = 0;
	char lastchar = 0;
	
	//maybe the buffer is all used up
	if(r->offset == r->linelen)
		if(!TReaderFillBuffer(r))
			return 0;
	
	//skip all separators
	if(!TReaderSkipSeparators(r))
		return 0;

	tokenEnd = strcspn(r->line+r->offset,r->separatorChars)+r->offset;
	if(r->line[tokenEnd] == '\n') r->lineNum++;

	//maybe the token was split during reading
	if(tokenEnd == r->linelen) {
		if(!strchr(r->separatorChars,r->line[tokenEnd-1])) {
			//the token might be incomplete
			unsigned int skip = (tokenEnd-1)-r->offset;
			if(r->offset == 0)
				TAbort("Text Stream Reader : token longer than what can be processed.");

			TReaderSeek(r,-(int)skip,SEEK_CUR);
			TReaderFillBuffer(r);

			tokenEnd = strcspn(r->line+skip,r->separatorChars)+skip;
			if(tokenEnd == r->linelen)
				TAbort("Text Stream Reader : token longer than what can be processed.");
		}
	}

	return tokenEnd;
}

const char *TReaderCurrentToken(TReader *r)
{
	if(!r) return 0;

	if (r->tokenend == 0)
		return TReaderNextToken(r);

	return &r->line[r->offset];
}

const char *TReaderNextToken(TReader *r)
{
	//TODO: Process Comments here

	if(!r) return 0;

	// Make sure there is more input on the line
	if(r->line && r->line[r->offset] == '\0') return 0;

	if(r->tokenend != r->offset) r->offset = r->tokenend + 1;

	if(r->line && r->line[r->offset] == '\0') return 0;

	r->tokenend = TReaderFindTokenEnd(r);

	if(r->tokenend == r->offset) return 0;

	r->line[r->tokenend] = '\0';

	return &r->line[r->offset];
}
