
#ifndef __included_terra_reader_h
#define __included_terra_reader_h

#define SEPARATOR_CHAR_LENGTH 16
#define NUM_COMMENT_FORMATS   5
#define SIZE_COMMENT_STRING   4

// In short, read content from a file

typedef struct _TReader TReader;

TReader *TReaderFileNew(const char *filename, unsigned char binary);
TReader *TReaderFilePointerNew(FILE *file);

void TReaderFree(TReader *r,unsigned char close);

unsigned int TReaderSize(TReader *r);
int TReaderSeek(TReader *r, int _offset, int _origin);
unsigned int TReaderTell(TReader *r);
unsigned char TReaderEof(TReader *r);

void TReaderSetSeparators(TReader *r,const char *_seperatorChars);
void TReaderSetCommentFormats(TReader *r,const char **_commentFormats,unsigned int size);

unsigned int TReaderGetLineNbr(TReader *r);

unsigned char TReaderRead8(TReader *r);
unsigned short TReaderRead16(TReader *r);
unsigned int TReaderRead32(TReader *r);
unsigned long long TReaderRead64(TReader *r);
unsigned int TReaderReadBlock(TReader *r,unsigned int _count, unsigned char *_buffer);

const char *TReaderCurrentToken(TReader *r);
const char *TReaderNextToken(TReader *r);

#endif
