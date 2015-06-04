
#ifndef __included_terra_filesys_h
#define __included_terra_filesys_h

size_t TFileSysListDirectory(char **output, const char *_dir, const char *_filter, unsigned char fullFilename);
size_t TFileSysListSubDirectoryNames(char **output, const char *_dir);

unsigned char TFileSysIsDirectory(const char *_fullPath);
unsigned char TFileSysFileExists(const char *_fullPath);
unsigned char TFileSysFilesIdentical(const char *_name1, const char *_name2);

char *TFileSysConcatPathsFetch(const char *(*func)(void *), void *data);
char *TFileSysConcatPathsArr(const char **paths, size_t size);
char *TFileSysConcatPaths(const char *_firstComponent, ...);

char *TFileSysGetParent(const char *_fullFilePath);
char *TFileSysGetDirectory(const char *_fullFilePath);
char *TFileSysGetDirname(const char *_fullFilePath);
const char *TFileSysGetFilename(const char *_fullFilePath);
const char *TFileSysGetExtension(const char *_fileFilePath);
char *TFileSysRemoveExtension(const char *_fullFileName);

char **TFileSysSplitPath(const char *path);
char **TFileSysSplitPathFull(const char *path,size_t *size);

unsigned char TFileSysCreateDirectory(const char *_directory);
unsigned char TFileSysCreateDirectoryRecursively (const char *_directory);
void TFileSysDelete(const char *_filename);

const char *TFileSysFindCaseInsensitive(const char *_fullPath);

#endif
