
#ifndef __included_terra_io_reader_h
#define __included_terra_io_reader_h

#define SEPARATOR_CHAR_LENGTH 16
#define NUM_COMMENT_FORMATS   5
#define SIZE_COMMENT_STRING   4

// In short, read content from a file

typedef struct _IOReader IOReader;

IOReader *file_reader_new(const char *filename, unsigned char binary);
IOReader *fp_reader_new(FILE *file);

void io_reader_free(IOReader *ior,unsigned char close);

unsigned int io_reader_size(IOReader *ior);
int io_reader_seek(IOReader *ior, int _offset, int _origin);
unsigned int io_reader_tell(IOReader *ior);
unsigned char io_reader_eof(IOReader *ior);

void io_reader_set_separators(IOReader *ior,const char *_seperatorChars);
void io_reader_set_comment_formats(IOReader *ior,const char **_commentFormats,unsigned int size);

unsigned int io_reader_get_line_nbr(IOReader *ior);

unsigned char io_reader_read8(IOReader *ior);
unsigned short io_reader_read16(IOReader *ior);
unsigned int io_reader_read32(IOReader *ior);
unsigned long long io_reader_read64(IOReader *ior);
unsigned int io_reader_read_block(IOReader *ior,unsigned int _count, unsigned char *_buffer);

const char *io_reader_current_token(IOReader *ior);
const char *io_reader_next_token(IOReader *ior);

#endif
