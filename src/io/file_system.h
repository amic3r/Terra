
#ifndef __included_terra_filesystem_h
#define __included_terra_filesystem_h

void terra_file_system_initialize(void);
void terra_file_system_destroy(void);

void terra_file_system_parse_archive(const char *filename);
SList *terra_file_system_list_archive(const char *_dir, const char *_filter, unsigned char fullFilename);

FILE *terra_file_system_get_file(const char *filename,const char *mode);
IOReader *terra_file_system_get_io_reader(const char *filename,unsigned char binarymode);
unsigned char *terra_file_system_get_buffered_file(const char *filename,unsigned char binarymode,unsigned int *size);

void terra_file_system_add_search_path(const char *path);
void terra_file_system_clear_search_path(void);

static inline const char *terra_file_system_get_application_path(void) { return "."; }

#endif
