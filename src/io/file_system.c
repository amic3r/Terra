
#include "stdafx.h"

#include "io_reader.h"
#include "structures/SList.h"

#include "file_system.h"

#ifdef _WINDOWS
#include <shlobj.h>
#endif

#include "utilities/filesys.h"

static SList *searchpaths = 0;
static char *savepath = 0;

void file_system_initialize(void)
{
	searchpaths = 0;
	file_system_add_search_path(file_system_get_application_path());
}

void file_system_destroy(void)
{
	slist_free(searchpaths,free);
	searchpaths = 0;
	free(savepath);
}

void file_system_parse_archive(const char *filename)
{
	AppAbort("File System: parse archive function not implemented");
}

SList *file_system_list_archive(const char *_dir, const char *_filter, unsigned char fullFilename)
{
	AppAbort("File System: list archive function not implemented");
	return 0;
}

void *testpath(void *searchpath, void *filename)
{
	char *fullFilename = ConcatPaths((const char *) searchpath, filename,NULL);
	if(FileExists(fullFilename)) return fullFilename;

	free(fullFilename);

	return 0;
}

FILE *file_system_get_file(const char *filename,const char *mode)
{
	char *found = 0;
	if(!filename) return 0;

	if(!mode) mode = "r";

	found = (char *) slist_foreach(searchpaths,testpath,(void *) filename);
	if(found) {
		FILE *f = fopen(found,mode);
		free(found);
		return f;
	}

	//TODO : check in archives

	return 0;
}

IOReader *file_system_get_io_reader(const char *filename,unsigned char binarymode)
{
	IOReader *reader = 0;

	FILE *f = file_system_get_file(filename,binarymode ? "rb" : "r");
	if(f) reader = fp_reader_new(f);

	return reader;
}

unsigned char *file_system_get_buffered_file(const char *filename,unsigned char binarymode,unsigned int *size)
{
	unsigned char *buffer = 0;
	unsigned int finalsize = 0;
	
	IOReader *ior = file_system_get_io_reader(filename,binarymode);
	if(!ior) return 0;

	finalsize = io_reader_size(ior);
	buffer = (unsigned char *) malloc(sizeof(unsigned char) * finalsize);
	*size = io_reader_read_block(ior,finalsize, buffer);

	io_reader_free(ior,1);

	return buffer;
}

void file_system_add_search_path(const char *path)
{
	if(!searchpaths) searchpaths = slist_new();

	slist_append(searchpaths,strdup(path));
}

void file_system_clear_search_path(void)
{
	slist_free(searchpaths,free);
	file_system_add_search_path(file_system_get_application_path());
}

const char *file_system_get_save_folder(void)
{
	terra_CreateDirectory(savepath);
	return savepath;
}

void file_system_set_save_folder(const char *path)
{
	free(savepath);
	savepath = strdup(path);
}
