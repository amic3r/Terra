
#ifndef __included_terra_io_writer_h
#define __included_terra_io_writer_h

#define NUM_COMMENT_FORMATS   5
#define SIZE_COMMENT_STRING   4

// In short, write content to a file

typedef struct _IOWriter IOWriter;

IOWriter *file_writer_new(const char *filename, unsigned char binary);
IOWriter *fp_writer_new(FILE *file);

void io_writer_free(IOWriter *iow,unsigned char close);

void io_writer_set_lineending(IOWriter *iow,const char *lineending);
void io_writer_set_comment_formats(IOWriter *iow,const char **_commentFormats,unsigned int size);

int io_writer_write8(IOWriter *iow,unsigned char data);
int io_writer_write16(IOWriter *iow,unsigned short data);
int io_writer_write32(IOWriter *iow,unsigned int data);
int io_writer_write64(IOWriter *iow,unsigned long long data);
int io_writer_write_block(IOWriter *iow,unsigned int _count, const void *_buffer);

void io_writer_write(IOWriter *iow,const char *format,...);
void io_writer_write_line(IOWriter *iow,const char *format,...);
void io_writer_write_comment(IOWriter *iow,unsigned int commentindex,const char *format,...);

void io_writer_write_valist(IOWriter *iow,const char *format, const va_list *list);
void io_writer_write_line_valist(IOWriter *iow,const char *format, const va_list *list);

#endif
