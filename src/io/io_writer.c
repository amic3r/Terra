
#include "stdafx.h"

#include "io_writer.h"

#include "thread/thread.h"

#define LINE_ENDING_LENGTH 4
#define DEFAULT_COMMENT_FORMAT "#","\n"
#define NUM_DEFAULT_FORMAT 2

struct _CommentFormat {
	char begin[SIZE_COMMENT_STRING];
	char end[SIZE_COMMENT_STRING];
};

struct _IOWriter {
	char lineending[LINE_ENDING_LENGTH];
	struct _CommentFormat commentFormat[NUM_COMMENT_FORMATS];

#ifndef __GNUC__
	ParticleMutex *iomut;
#endif

	FILE *file;
};

IOWriter *file_writer_new(const char *filename, unsigned char binary)
{
	return fp_writer_new(fopen(filename,binary ? "wb" : "w"));
}

IOWriter *fp_writer_new(FILE *file)
{
	IOWriter *iow;
	const char *formats[] = {DEFAULT_COMMENT_FORMAT};
	
	if(!file) return 0;
	
	iow = (IOWriter *) malloc(sizeof(IOWriter));
	if(!iow) return 0;
	
	iow->file = file;

#if defined (_MSC_VER) || defined (_WINDOWS)
	io_writer_set_lineending(iow,"\r\n");
#else
	io_writer_set_lineending(iow,"\n");
#endif
	
	io_writer_set_comment_formats(iow,formats,NUM_DEFAULT_FORMAT);

#ifndef __GNUC__
	iow->iomut = particle_mutex_new(PARTICLE_MUTEX_TYPE_RECURSIVE);
#endif

	return iow;
}

void io_writer_free(IOWriter *iow,unsigned char close)
{
	if(iow) {
		if(close) fclose(iow->file);
#ifndef __GNUC__
		particle_mutex_free(iow->iomut);
#endif

		free(iow);
	}
}

void io_writer_set_lineending(IOWriter *iow,const char *lineending)
{
	if(iow && lineending) {
		strncpy(iow->lineending,lineending,LINE_ENDING_LENGTH);
		iow->lineending[LINE_ENDING_LENGTH-1] = '\0';
	}
}

void io_writer_set_comment_formats(IOWriter *iow,const char **_commentFormats,unsigned int size)
{
	if(!iow || size & 1) return;

	{
		unsigned int i = 0,j = 0;
		for(;i < size && j < NUM_COMMENT_FORMATS; i+=2, ++j) {
			strncpy(iow->commentFormat[j].begin,_commentFormats[i],SIZE_COMMENT_STRING-1);
			strncpy(iow->commentFormat[j].end,_commentFormats[i+1],SIZE_COMMENT_STRING-1);
		}

		for(; j < NUM_COMMENT_FORMATS; ++j) {
			strncpy(iow->commentFormat[j].begin,"\0",SIZE_COMMENT_STRING-1);
			strncpy(iow->commentFormat[j].end,"\0",SIZE_COMMENT_STRING-1);
		}
	}
}

int io_writer_write8(IOWriter *iow,unsigned char data)
{
	int result = 0;

	if(iow) {
#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned char), iow->file);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}

	return result;
}

int io_writer_write16(IOWriter *iow,unsigned short data)
{
	int result = 0;

	if(iow) {
#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned short), iow->file);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}

	return result;
}

int io_writer_write32(IOWriter *iow,unsigned int data)
{
	int result = 0;

	if(iow) {
#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned int), iow->file);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}

	return result;
}

int io_writer_write64(IOWriter *iow,unsigned long long data)
{
	int result = 0;

	if(iow) {
#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		result = fwrite(&data, 1, sizeof(unsigned long long), iow->file);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}

	return result;
}

int io_writer_write_block(IOWriter *iow,unsigned int _count, const void *_buffer)
{
	int result = 0;

	if(iow) {
#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		result = fwrite(_buffer, 1, _count, iow->file);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}

	return result;
}

void io_writer_write(IOWriter *iow,const char *format,...)
{
	if(iow && format) {
		
		va_list args;
		va_start(args, format);

#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		vfprintf(iow->file,format,args);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
		va_end(args);
	}
}

void io_writer_write_line(IOWriter *iow,const char *format,...)
{
	if(iow && format) {
		
		va_list args;
		va_start(args, format);

#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		vfprintf(iow->file,format,args);
		fprintf(iow->file, "%s", iow->lineending);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
		va_end(args);
	}
}

void io_writer_write_comment(IOWriter *iow,unsigned int commentindex,const char *format,...)
{
	if(iow && format) {
		struct _CommentFormat *comment;
		va_list args;

		if(commentindex > NUM_COMMENT_FORMATS) return;
		comment = &iow->commentFormat[commentindex];
		if (comment->begin[0] == '\0') return;
		
		va_start(args, format);

#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		fprintf(iow->file, "%s", comment->begin);
		vfprintf(iow->file,format,args);
		fprintf(iow->file, "%s",comment->end);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
		va_end(args);
	}
}

void io_writer_write_valist(IOWriter *iow,const char *format, const va_list *list)
{
	if(iow && format) {

#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		vfprintf(iow->file,format,*list);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}
}

void io_writer_write_line_valist(IOWriter *iow,const char *format, const va_list *list)
{
	if(iow && format) {

#ifndef __GNUC__
		particle_mutex_lock(iow->iomut);
#endif
		vfprintf(iow->file,format,*list);
		fprintf(iow->file, "%s", iow->lineending);

#ifndef __GNUC__
		particle_mutex_unlock(iow->iomut);
#endif
	}
}
