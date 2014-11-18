
#ifndef __included_terra_writer_h
#define __included_terra_writer_h

#define NUM_COMMENT_FORMATS   5
#define SIZE_COMMENT_STRING   4

// In short, write content to a file

typedef struct _TWriter TWriter;

TWriter *TWriterFileNew(const char *filename, unsigned char binary);
TWriter *TWriterFilePointerNew(FILE *file);

void TWriterFree(TWriter *w,unsigned char close);

void TWriterSetLineEnding(TWriter *w,const char *lineending);
void TWriterSetCommentFormats(TWriter *w,const char **_commentFormats,unsigned int size);

int TWriterWrite8(TWriter *w,unsigned char data);
int TWriterWrite16(TWriter *w,unsigned short data);
int TWriterWrite32(TWriter *w,unsigned int data);
int TWriterWrite64(TWriter *w,unsigned long long data);
int TWriterWriteBlock(TWriter *w,unsigned int _count, const void *_buffer);

void TWriterWrite(TWriter *w,const char *format,...);
void TWriterWriteLine(TWriter *w,const char *format,...);
void TWriterWriteComment(TWriter *w,unsigned int commentindex,const char *format,...);

void TWriterWriteV(TWriter *w,const char *format, va_list list);
void TWriterWriteLineV(TWriter *w,const char *format, va_list list);

#endif
