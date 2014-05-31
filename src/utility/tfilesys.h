
#ifndef __included_terra_filesys_h
#define __included_terra_filesys_h

#include "structure/tlist.h"

int TFileSysListDirectory(TSList *list, const char *_dir, const char *_filter, unsigned char fullFilename);
int TFileSysListSubDirectoryNames(TSList *list, const char *_dir);

unsigned char TFileSysIsDirectory(const char *_fullPath);
unsigned char TFileSysFileExists(const char *_fullPath);
unsigned char TFileSysFilesIdentical(const char *_name1, const char *_name2);

char *TFileSysConcatPathsArr(const char **paths, size_t size);
char *TFileSysConcatPathsList(TSList *list);
char *TFileSysConcatPaths(const char *_firstComponent, ...);

char *TFileSysGetDirectoryPart(const char *_fullFilePath);
const char *TFileSysGetFilenamePart(const char *_fullFilePath);
const char *TFileSysGetExtensionPart(const char *_fileFilePath);
char *TFileSysRemoveExtension(const char *_fullFileName);

char **TFileSysSplitPath(const char *path, size_t *size);

unsigned char TFileSysCreateDirectory(const char *_directory);
unsigned char TFileSysCreateDirectoryRecursively (const char *_directory);
void TFileSysDelete(const char *_filename);

const char *TFileSysFindCaseInsensitive(const char *_fullPath);

#endif
