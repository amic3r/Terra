
#ifndef __included_terra_reader_h
#define __included_terra_reader_h

// In short, read and write content to a buffer

typedef struct _TRW TRW;

typedef struct _TRWOps {
	size_t (*size) (TRW *context);

    int (*seek) (TRW *context, int offset, int origin);
	int (*tell) (TRW *context);

    size_t (*read) (TRW *context, void *buffer, size_t size);
    size_t (*write) (TRW *context, const void *buffer, size_t size);

    int (*close) (TRW *context);
} TRWOps;

typedef void * TRWContent;

struct _TRW {
	TRWOps operations;
	TRWContent content;
};

TRW *TRWFromFile(const char *filename, const char *mode);
TRW *TRWFromFilePointer(FILE *file, unsigned char autoclose);
TRW *TRWFromMem(char *buffer, int size);
TRW *TRWFromConstMem(const char *buffer, int size);
TRW *TRWFromContent(TRWContent content);

void TRWFree(TRW *context);

void TRWSetOps(TRW *context, const TRWOps *ops);

size_t TRWSize(TRW *context);
int TRWSeek(TRW *context, int offset, int origin);
int TRWTell(TRW *context);

unsigned char TRWRead8(TRW *context);
unsigned short TRWRead16(TRW *context);
unsigned int TRWRead32(TRW *context);
unsigned long long TRWRead64(TRW *context);
size_t TRWReadBlock(TRW *context, char *buffer, size_t size);

int TRWWrite8(TRW *context, unsigned char data);
int TRWWrite16(TRW *context, unsigned short data);
int TRWWrite32(TRW *context, unsigned int data);
int TRWWrite64(TRW *context, unsigned long long data);
int TRWWriteBlock(TRW *context, const void *buffer, size_t size);

void TRWWrite(TRW *context, const char *format,...);
void TRWWriteV(TRW *context, const char *format, va_list list);

#endif
