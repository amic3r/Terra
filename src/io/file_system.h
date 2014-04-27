
#ifndef __included_terra_filesystem_h
#define __included_terra_filesystem_h

void file_system_initialize(void);
void file_system_destroy(void);

void file_system_parse_archive(const char *filename);
SList *file_system_list_archive(const char *_dir, const char *_filter, unsigned char fullFilename);

FILE *file_system_get_file(const char *filename,const char *mode);
IOReader *file_system_get_io_reader(const char *filename,unsigned char binarymode);
unsigned char *file_system_get_buffered_file(const char *filename,unsigned char binarymode,unsigned int *size);

void file_system_add_search_path(const char *path);
void file_system_clear_search_path(void);

const char *file_system_get_save_folder(void);
void file_system_set_save_folder(const char *path);

__inline const char *file_system_get_application_path(void) { return "."; }

#endif
