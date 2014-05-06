
#ifndef __included_terra_filesys_utils
#define __included_terra_filesys_utils

#include "structure/SList.h"

void ListDirectory(SList *list, const char *_dir, const char *_filter, unsigned char fullFilename);
void ListSubDirectoryNames(SList *list, const char *_dir);

unsigned char IsDirectory(const char *_fullPath);
unsigned char FileExists(const char *_fullPath);
unsigned char FilesIdentical(const char *_name1, const char *_name2);

char *ConcatPathsList(SList *list);
char *ConcatPaths(const char *_firstComponent, ...);
char *GetDirectoryPart(const char *_fullFilePath);
const char *GetFilenamePart(const char *_fullFilePath);
const char *GetExtensionPart(const char *_fileFilePath);
char *RemoveExtension(const char *_fullFileName);

int splitPath(SList *list,const char *path);

unsigned char terra_CreateDirectory(const char *_directory);
unsigned char CreateDirectoryRecursively (const char *_directory);
void DeleteThisFile(const char *_filename);

const char *FindCaseInsensitive(const char *_fullPath);

#endif
