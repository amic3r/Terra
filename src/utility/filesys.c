
#include "stdafx.h"

#ifdef _WINDOWS
#include <io.h>
#include <direct.h>
#endif

#include "filesys.h"

#include "string.h"

#define FILE_PATH_BUFFER_SIZE 256
static char s_filePathBuffer[FILE_PATH_BUFFER_SIZE + 1];

static unsigned char FilterMatch(const char *_filename, const char *_filter)
{
	const char *_filterptr = 0;

	while(*_filename && *_filter) {
		if(tolower(*_filename)== tolower(*_filter) || *_filter == '?') {
			_filename++;
			_filter++;
		} else if (*_filter == '*') {
			if(!*(++_filter)) return 1;
			_filterptr = _filter;
			_filename++;
		} else if(_filterptr) {
			_filter = _filterptr;
			_filename++;
		} else {
			return 0;
		}
	}

	return !*_filename && !*_filter;
}

/*static unsigned char FilterMatch(const char *_filename, const char *_filter)
{
	while(*_filename && tolower(*_filename)== tolower(*_filter)) {
		_filename++;
		_filter++;
	}

	if(tolower(*_filename)== tolower(*_filter))
		return 1;

	switch(*_filter++) {
		case '*':
			while(*_filename) {
				_filename++;
				if(FilterMatch(_filename, _filter))
					return 1;
			}
			return 0;

		case '?':
			if(!*_filename)
				return 0;
			_filename++;
			return FilterMatch(_filename, _filter);
		default:
			return 0;
	}
}*/

void ListDirectory(SList *list, const char *_dir, const char *_filter, unsigned char _fullFilename)
{
#ifdef _WINDOWS
	unsigned int dirlen = 0;
	long fileindex = -1;
	char *searchstring = 0;
	struct _finddata_t thisfile;
	char *dir = 0;
#endif

	if(!list) return;
	if(_filter == NULL || _filter[0] == '\0') _filter = "*";
	if(_dir == NULL || _dir[0] == '\0')       _dir = ".";

	// Now add on all files found locally
#ifdef _WINDOWS
	dirlen = strlen(_dir);
	dir = ConcatPaths(_dir, _filter, NULL);
	AppAssert(dir);

	fileindex = _findfirst(dir, &thisfile);
	free(dir);

	if(fileindex != -1) {
		do {
			if(!(thisfile.attrib & _A_SUBDIR)) {
				char *newname = 0;
				int len = strlen(thisfile.name) + 1;
				if(_fullFilename) len += dirlen + 1;

				newname = (char *) malloc(len);
				if(_fullFilename)
					sprintf(newname, "%s/%s", _dir, thisfile.name);
				else
					sprintf(newname, "%s", thisfile.name);

				slist_append(list,newname);
			}
		} while(!_findnext(fileindex, &thisfile));
	}
#else
	DIR *dir = opendir(_dir);
	if(dir == NULL)
		return result;
	for(struct dirent *entry;(entry = readdir(dir))!= NULL;) {
		if(FilterMatch(entry->d_name, _filter)) {
			char fullname[strlen(_dir) + strlen(entry->d_name) + 2];
			sprintf(fullname, "%s%s%s", _dir, _dir[0] ? "/" : "", entry->d_name);
			if(!IsDirectory(fullname))
				slist_append(list,strdup(_fullFilename ? fullname : entry->d_name));
		}
	}
	closedir(dir);
#endif
}

void ListSubDirectoryNames(SList *list, const char *_dir)
{
#ifdef _WINDOWS
	struct _finddata_t thisfile;
	char *dir = ConcatPaths(_dir, "*", NULL);
	long fileindex = _findfirst(dir, &thisfile);

	if(!list) return;
	if(fileindex != -1) {
		while(!_findnext(fileindex, &thisfile)) {
			if(thisfile.name[0] != '.' && thisfile.attrib & _A_SUBDIR) {
				char *newname = strdup(thisfile.name);
				
				slist_append(list,newname);
			}
		} 
	}

	free(dir);
#else
	DIR *dir = opendir(_dir);

	if(!list) return;
	if(dir == NULL)
		return result;
	for(struct dirent *entry;(entry = readdir(dir))!= NULL;) {
		if(entry->d_name[0] == '.')
			continue;

		char fullname[strlen(_dir) + strlen(entry->d_name) + 2];
		sprintf(fullname, "%s%s%s", _dir, _dir[0] ? "/" : "", entry->d_name);

		if(IsDirectory(fullname))
			slist_append(list,strdup(entry->d_name));
	}
	closedir(dir);
#endif
}

unsigned char FileExists(const char *_fullPath)
{
	FILE *f = fopen(FindCaseInsensitive(_fullPath), "r");
	if(f) {
		fclose(f);
		return 1;
	}

	return 0;
}

char *ConcatPathsList(SList *list)
{
	const char *component = (const char *) slist_first(list);
	char *buffer, *returnBuffer;
	unsigned int bufferlen = 0;
	
	buffer = strdup(component);
	bufferlen = strlen(buffer) + 1;
	
	while(component = (const char *) slist_next(list))
	{
		if(!strcmp(component,"..")) {
			char *lastpath = strrchr(buffer,'/');
			if(!lastpath) lastpath = buffer;
			if(!lastpath+1) {
				*lastpath = 0;
				lastpath = strrchr(buffer,'/');
			}
			*lastpath = 0;
			bufferlen = lastpath-buffer + 1;
		} else if(strcmp(component,".")) {
			bufferlen += strlen(component) + 1;
			buffer =(char *)realloc(buffer, bufferlen);
			if(buffer[0]) strcat(buffer, "/");
			strcat(buffer, component);
		}
	}
	
	returnBuffer = strdup(buffer);
	free(buffer);
	return returnBuffer;
}

char *ConcatPaths(const char *_firstComponent, ...)
{
	va_list components;
	const char *component;
	char *buffer, *returnBuffer;
	unsigned int bufferlen = 0;
	
	buffer = strdup(_firstComponent);
	bufferlen = strlen(buffer) + 1;
	
	va_start(components, _firstComponent);
	while(component = va_arg(components, const char *))
	{
		if(!strcmp(component,"..")) {
			char *lastpath = strrchr(buffer,'/');
			if(!lastpath) lastpath = buffer;
			*lastpath = 0;
			bufferlen = lastpath-buffer + 1;

		} else if(strcmp(component,".")) {
			bufferlen += strlen(component) + 1;
			buffer =(char *)realloc(buffer, bufferlen);
			if(buffer[0]) strcat(buffer, "/");
			strcat(buffer, component);
		}
	}
	va_end(components);
	
	returnBuffer = strdup(buffer);
	free(buffer);
	return returnBuffer;
}

char *GetDirectoryPart(const char *_fullFilePath)
{
	char *finalSlash = 0;
	strncpy(s_filePathBuffer, _fullFilePath, FILE_PATH_BUFFER_SIZE);

	finalSlash = strrchr(s_filePathBuffer,'/');
	if(finalSlash) {
		*(finalSlash+1)= '\x0';
		return s_filePathBuffer;
	}

	return 0;
}

const char *GetFilenamePart(const char *_fullFilePath)
{
	return strrchr(_fullFilePath, '/') + 1;
}

const char *GetExtensionPart(const char *_fullFilePath)
{
	return strrchr(_fullFilePath, '.') + 1;
}

char *RemoveExtension(const char *_fullFileName)
{
	char *dot = 0;
	strcpy(s_filePathBuffer, _fullFileName);

	dot = strrchr(s_filePathBuffer, '.');
	if(dot)				*dot = '\x0';
	return s_filePathBuffer;
}

int splitPath(SList *list,const char *path)
{
	return string_split(list, path, "/");
}

unsigned char FilesIdentical(const char *_name1, const char *_name2)
{
	FILE *in1 = fopen(_name1, "r");
	FILE *in2 = fopen(_name2, "r");
	unsigned char rv = 1;
	unsigned char exitNow = 0;

	if(!in1 || !in2) {
		rv = 0;
		exitNow = 1;
	}

	while(!exitNow && !feof(in1)&& !feof(in2)) {
		int a = fgetc(in1);
		int b = fgetc(in2);
		if(a != b) {
			rv = 0;
			exitNow = 1;
			break;
		}
	}

	if(!exitNow && feof(in1)!= feof(in2))
		rv = 0;

	if(in1)fclose(in1);
	if(in2)fclose(in2);

	return rv;
}

unsigned char terra_CreateDirectory(const char *_directory)
{
#ifdef _WINDOWS
	int result = _mkdir(_directory);
	if(result == 0)									return 0;	// Directory was created
	if(result == -1 && errno == 17 /* EEXIST */)	return 0;	// Directory already exists
	return 1;
#else
	if(access(_directory, W_OK|X_OK|R_OK)== 0)
		return 0;
	return mkdir(_directory, 0755);
#endif
}

unsigned char CreateDirectoryRecursively(const char *_directory)
{
	const char *p;
	char *buffer;
	unsigned char error = 0;
	
	if(strlen(_directory)== 0)
		return 0;

	buffer = (char *) malloc(strlen(_directory) + 1);
	AppAssert(buffer);
	p = _directory;
	if(*p == '/')
		p++;
	
	p = strchr(p, '/');
	while(p && !error) {
		memcpy(buffer, _directory, p - _directory);
		buffer[ p-_directory ] = '\0';
		error = !terra_CreateDirectory(buffer);
		p = strchr(p+1, '/');
	}
	
	return error ? 1 : terra_CreateDirectory(_directory);
}

void DeleteThisFile(const char *_filename)
{
#ifdef WIN32
	DeleteFile(_filename);
#else
	unlink(_filename);
#endif
}

unsigned char IsDirectory(const char *_fullPath)
{
#ifdef WIN32
	unsigned long dwAttrib = GetFileAttributes(_fullPath);
	return (dwAttrib != -1 && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
	struct stat s;
	int rc = stat(_fullPath, &s);
	if(rc != 0)
		return 0;
	return(s.st_mode & S_IFDIR);
#endif
}

const char *FindCaseInsensitive(const char *_fullPath)
{
#ifndef TARGET_OS_LINUX
	return _fullPath;
#else
	if(!_fullPath)
		return NULL;

	static char retval[PATH_MAX];
	char *dir = NULL, *file = NULL;

	// Make our own copy of the result, since GetDirectoryPart
	// and GetFilenamePart use the same variable for temp
	// storage.
	if((dir = GetDirectoryPart(_fullPath))!= NULL)		dir = newStr(dir);

	// No directory provided. Assume working directory.
	if(!dir)											file = newStr(_fullPath);
	else {
		dir[strlen(dir) - 1] = '\0';
		file = newStr(GetFilenamePart(_fullPath));
	}
	LList <char *> *files = ListDirectory(dir, file);

	delete [] dir; delete [] file; dir = file = NULL;

	// We shouldn't have found more than one match.
	AppAssert(files->Size()<= 1);

	// No results, so maybe the file does not exist.
	if(files->Size()== 0)
		return _fullPath;

	// Copy the corrected path back, and prepare to return it.
	memset(retval, 0, sizeof(retval));
	AppAssert(strlen(files->GetData(0))< PATH_MAX);
	strcpy(retval, files->GetData(0));

	// Negate the possibility of a memory access violation.
	// This way, we can simply strcpy the result inline without
	// worrying about a buffer overflow.
	AppAssert(strlen(retval)== strlen(_fullPath));

	while(files->Size()) {
		char *data = files->GetData(0);
		files->RemoveData(0);
		delete [] data;
	}

	delete files;

	return retval;
#endif
}
