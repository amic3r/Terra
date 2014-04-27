
#include "stdafx.h"

#include "io_reader.h"

#include "particle/particle.h"

#define DEFAULT_SEPERATOR_CHARS " \t\n\r:,"

#define DEFAULT_COMMENT_FORMAT "#","\n"
#define NUM_DEFAULT_FORMAT 2

#define INIT_LINE_SIZE 128
#define LINE_SIZE_LIMIT 2048

struct _CommentFormat {
	char begin[SIZE_COMMENT_STRING];
	char end[SIZE_COMMENT_STRING];
};

struct _IOReader {
	char *line;
	unsigned int linelen;			// string length without '\0'

	char separatorChars[SEPARATOR_CHAR_LENGTH];
	struct _CommentFormat commentFormat[NUM_COMMENT_FORMATS];

	unsigned int lineNum;

	unsigned int offset;
	unsigned int tokenend;

#ifndef __GNUC__
	ParticleMutex *iomut;
#endif

	FILE *file;
};

unsigned char io_reader_grow(IOReader *ior,unsigned int size)
{
	char *biggerline = (char*) realloc(ior->line, ior->linelen+1+size);
	if(!biggerline) return 1;

	ior->line = biggerline;
	ior->linelen += size;

	return 0;
}

unsigned int io_reader_double_size(IOReader *ior)
{
	if(!ior) return 1;

	if(ior->linelen == 0) {
		ior->line = (char*) malloc(INIT_LINE_SIZE);
		if(!ior->line) return 1;

		ior->linelen = INIT_LINE_SIZE-1;
		return 0;
	}

	if(ior->linelen + 1 >= LINE_SIZE_LIMIT) return 2;
	
	return io_reader_grow(ior,ior->linelen+1);
}

IOReader *file_reader_new(const char *filename, unsigned char binary)
{
	return fp_reader_new(fopen(filename,binary ? "rb" : "r"));
}

IOReader *fp_reader_new(FILE *file)
{
	IOReader *ior;
	const char *formats[] = {DEFAULT_COMMENT_FORMAT};
	
	if(!file) return 0;
	
	ior = (IOReader *) malloc(sizeof(IOReader));
	if(!ior) return 0;
	
	ior->file = file;

	ior->offset = ior->linelen = ior->lineNum = ior->tokenend = 0;
	ior->line = 0;

	strncpy(ior->separatorChars, DEFAULT_SEPERATOR_CHARS, SEPARATOR_CHAR_LENGTH-1);
	io_reader_set_comment_formats(ior,formats,NUM_DEFAULT_FORMAT);

#ifndef __GNUC__
	ior->iomut = particle_mutex_new(PARTICLE_MUTEX_TYPE_RECURSIVE);
#endif

	return ior;
}

void io_reader_free(IOReader *ior, unsigned char close)
{
	if(ior) {
		if(close) fclose(ior->file);

		free(ior->line);
		ior->line = 0;

#ifndef __GNUC__
		particle_mutex_free(ior->iomut);
#endif

		free(ior);
	}
}

unsigned int io_reader_size(IOReader *ior)
{
	unsigned int now = 0,size = 0;
	if(!ior) return 0;

	now = io_reader_tell(ior);
	fseek(ior->file, 0, SEEK_END);
	size = io_reader_tell(ior);
	fseek(ior->file, now, SEEK_SET);

	return size;
}

int io_reader_seek(IOReader *ior, int _offset, int _origin)
{
	if(!ior) return 1;

	if(_origin == SEEK_CUR)
		ior->offset += _offset;
	else if(_origin == SEEK_END)
		ior->offset = io_reader_size(ior) - _offset;
	else if(_origin == SEEK_SET)
		ior->offset = _offset;

	return fseek(ior->file, _offset, _origin);
}

unsigned int io_reader_tell(IOReader *ior)
{
	if(!ior) return 0;

	return ftell(ior->file);
}

unsigned char io_reader_eof(IOReader *ior)
{
	if(!ior) return 1;

	return feof(ior->file);
}

void io_reader_set_separators(IOReader *ior,const char *_seperatorChars)
{
	if(!ior) return;

	strncpy(ior->separatorChars, _seperatorChars, SEPARATOR_CHAR_LENGTH-1);
	ior->separatorChars[SEPARATOR_CHAR_LENGTH-1] = '\0';
}

void io_reader_set_comment_formats(IOReader *ior,const char **_commentFormats,unsigned int size)
{
	if(!ior || size & 1) return;

	{
		unsigned int i = 0,j = 0;
		for(;i < size && j < NUM_COMMENT_FORMATS; i+=2, ++j) {
			strncpy(ior->commentFormat[j].begin,_commentFormats[i],SIZE_COMMENT_STRING-1);
			strncpy(ior->commentFormat[j].end,_commentFormats[i+1],SIZE_COMMENT_STRING-1);
		}

		for(; j < NUM_COMMENT_FORMATS; ++j) {
			strncpy(ior->commentFormat[j].begin,"\0",SIZE_COMMENT_STRING-1);
			strncpy(ior->commentFormat[j].end,"\0",SIZE_COMMENT_STRING-1);
		}
	}
}

unsigned int io_reader_get_line_nbr(IOReader *ior)
{
	return ior ? ior->lineNum : 0;
}

unsigned char io_reader_read8(IOReader *ior)
{
	unsigned char content = EOF;

	if(ior) {
#ifndef __GNUC__
		particle_mutex_lock(ior->iomut);
#endif

		fread(&content, 1, sizeof(unsigned char), ior->file);

#ifndef __GNUC__
		particle_mutex_unlock(ior->iomut);
#endif
		ior->offset += 1;
	}

	return content;
}

unsigned short io_reader_read16(IOReader *ior)
{
	unsigned short content = EOF;

	if(ior) {
#ifndef __GNUC__
		particle_mutex_lock(ior->iomut);
#endif

		fread(&content, 1, sizeof(unsigned short), ior->file);

#ifndef __GNUC__
		particle_mutex_unlock(ior->iomut);
#endif

		ior->offset += 2;
	}

	return content;
}

unsigned int io_reader_read32(IOReader *ior)
{
	unsigned int content = EOF;

	if(ior) {
#ifndef __GNUC__
		particle_mutex_lock(ior->iomut);
#endif

		fread(&content, 1, sizeof(unsigned int), ior->file);

#ifndef __GNUC__
		particle_mutex_unlock(ior->iomut);
#endif
		ior->offset += 4;
	}

	return content;
}

unsigned long long io_reader_read64(IOReader *ior)
{
	unsigned long long content = EOF;

	if(ior) {
#ifndef __GNUC__
		particle_mutex_lock(ior->iomut);
#endif

		fread(&content, 1, sizeof(unsigned long long), ior->file);

#ifndef __GNUC__
		particle_mutex_unlock(ior->iomut);
#endif
		ior->offset += 8;
	}

	return content;
}

unsigned int io_reader_read_block(IOReader *ior,unsigned int _count, unsigned char *_buffer)
{
	unsigned int result = 0;
	if(!ior || !_buffer || _count == 0) return 0;

#ifndef __GNUC__
		particle_mutex_lock(ior->iomut);
#endif

	result = fread(_buffer, 1, _count, ior->file);

#ifndef __GNUC__
		particle_mutex_unlock(ior->iomut);
#endif

	ior->offset += result;
	_buffer[result++] = '\0';

	return result;
}

unsigned char io_reader_next_line(IOReader *ior)
{
	unsigned char found = 0;
	unsigned int result = 0;
	char *ptr = ior->line;

	if(feof(ior->file)) {
		*ptr = 0;
		return 0;
	}

	result = fread(ptr, 1, ior->linelen, ior->file);
	ptr[result] = 0;

	while(!feof(ior->file)) {
		unsigned int oldLineLen = ior->linelen;
		unsigned int status = io_reader_double_size(ior);
		if(status == 0) {
			ptr += oldLineLen;
			result = fread(ptr,oldLineLen, 1, ior->file);
			ptr[result] = 0;
		} else if(status == 2) {
			//limit reached - we cut the line here
			break;
		} else {
			//mem allocation error
			AppAbort("Text Stream Reader: memory allocation failed");
		}
	}

	return 1;
}

unsigned char io_reader_fill_buffer(IOReader *ior)
{
	unsigned int res = 0;
	if(!ior) return 0;

	ior->offset = 0;

#ifndef __GNUC__
	particle_mutex_lock(ior->iomut);
#endif

	if(io_reader_eof(ior)) {
		*ior->line = 0;
		return 0;
	}

	if(!ior->line) io_reader_double_size(ior);

	res = io_reader_next_line(ior);

#ifndef __GNUC__
	particle_mutex_unlock(ior->iomut);
#endif

	return res;
}

unsigned char io_reader_skip_separators(IOReader *ior)
{
	while(strchr(ior->separatorChars,ior->line[ior->offset])) {
		if(ior->line[ior->offset] == '\n') ior->lineNum++;
		if(++ior->offset == ior->linelen)
			if(!io_reader_fill_buffer(ior))
				return 0;
	}

	return 1;
}

unsigned int io_reader_find_token_end(IOReader *ior)
{
	unsigned int tokenEnd = 0;
	char lastchar = 0;
	
	//maybe the buffer is all used up
	if(ior->offset == ior->linelen)
		if(!io_reader_fill_buffer(ior))
			return 0;
	
	//skip all separators
	if(!io_reader_skip_separators(ior))
		return 0;

	tokenEnd = strcspn(ior->line+ior->offset,ior->separatorChars)+ior->offset;
	if(ior->line[tokenEnd] == '\n') ior->lineNum++;

	//maybe the token was split during reading
	if(tokenEnd == ior->linelen) {
		if(!strchr(ior->separatorChars,ior->line[tokenEnd-1])) {
			//the token might be incomplete
			unsigned int skip = (tokenEnd-1)-ior->offset;
			if(ior->offset == 0)
				AppAbort("Text Stream Reader : token longer than what can be processed.");

			io_reader_seek(ior,-(int)skip,SEEK_CUR);
			io_reader_fill_buffer(ior);

			tokenEnd = strcspn(ior->line+skip,ior->separatorChars)+skip;
			if(tokenEnd == ior->linelen)
				AppAbort("Text Stream Reader : token longer than what can be processed.");
		}
	}

	return tokenEnd;
}

const char *io_reader_current_token(IOReader *ior)
{
	if(!ior) return 0;

	if (ior->tokenend == 0)
		return io_reader_next_token(ior);

	return &ior->line[ior->offset];
}

const char *io_reader_next_token(IOReader *ior)
{
	//TODO: Process Comments here

	if(!ior) return 0;

	// Make sure there is more input on the line
	if(ior->line && ior->line[ior->offset] == '\0') return 0;

	if(ior->tokenend != ior->offset) ior->offset = ior->tokenend + 1;

	if(ior->line && ior->line[ior->offset] == '\0') return 0;

	ior->tokenend = io_reader_find_token_end(ior);

	if(ior->tokenend == ior->offset) return 0;

	ior->line[ior->tokenend] = '\0';

	return &ior->line[ior->offset];
}
